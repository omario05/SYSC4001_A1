/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 * @author Omar Elhogaraty, 101302440
 * @author Ahmad El-Jabi, 101303269
 * @version Spetember 22, 2025
 */

#include "interrupts.hpp" //contains helper functions

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);
    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    long total_cpu_time = 0;
    int context_save_time = 10;
    int ISR_ACTIVITY = 40;
    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity=="CPU"){
            execution+=std::to_string(total_cpu_time)+", "+std::to_string(duration_intr)+", CPU Burst\n";
            total_cpu_time+=duration_intr;
        } else if (activity=="SYSCALL"){
            int device_num=duration_intr;
            auto [interrupt_seq, updated_time]=intr_boilerplate(total_cpu_time, device_num, context_save_time, vectors);
            execution+=interrupt_seq;
            total_cpu_time=updated_time;
            execution+=std::to_string(total_cpu_time)+", "+std::to_string(ISR_ACTIVITY)+", SYSCALL: run the ISR (device driver)\n";
            total_cpu_time+=ISR_ACTIVITY;
            execution+=std::to_string(total_cpu_time)+", "+std::to_string(ISR_ACTIVITY)+", transfer data from device to memory\n";
            total_cpu_time+=ISR_ACTIVITY;
            execution+=std::to_string(total_cpu_time)+", "+std::to_string(ISR_ACTIVITY)+", check for errors\n";
            total_cpu_time+=ISR_ACTIVITY;
            execution+=std::to_string(total_cpu_time)+", 1, IRET\n";
            total_cpu_time+=1;
        } else if (activity=="END_IO"){
            int device_num=duration_intr;
            auto [interrupt_seq, updated_time]=intr_boilerplate(total_cpu_time, device_num, context_save_time, vectors);
            execution+=interrupt_seq;
            total_cpu_time=updated_time;
            execution+=std::to_string(total_cpu_time)+", "+std::to_string(ISR_ACTIVITY)+", END_IO: run the ISR (device driver)\n";
            total_cpu_time+=ISR_ACTIVITY;
            execution+=std::to_string(total_cpu_time)+", "+std::to_string(ISR_ACTIVITY)+", transfer data from device to memory\n";
            total_cpu_time+=ISR_ACTIVITY;
            execution+=std::to_string(total_cpu_time)+", "+std::to_string(ISR_ACTIVITY)+", check for errors\n";
            total_cpu_time+=ISR_ACTIVITY;
            execution+= std::to_string(total_cpu_time)+", 1, IRET\n";
            total_cpu_time += 1;    
        }
        /************************************************************************/
    }
    input_file.close();
    write_output(execution);
    return 0;
}
