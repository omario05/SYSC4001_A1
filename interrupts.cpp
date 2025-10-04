/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 * @author Omar Elhogaraty, 101302440
 * @author Ahmad El-Jabi, 101303269
 */

#include "interrupts.hpp"

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);
    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    long current_time = 0;
    int context_save_time = 10;
    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity=="CPU"){
            execution+=std::to_string(current_time)+", "+std::to_string(duration_intr)+", CPU Burst\n";
            current_time+=duration_intr;
        }
        else if (activity=="SYSCALL"){
            int device_num=duration_intr;
            auto [interrupt_seq, updated_time]=intr_boilerplate(current_time, device_num, context_save_time, vectors);
            execution+=interrupt_seq;
            current_time=updated_time;
        }
        else if (activity=="END_IO"){
            int device_num=duration_intr;
            auto [interrupt_seq, updated_time]=intr_boilerplate(current_time, device_num, context_save_time, vectors);
            execution+=interrupt_seq;
            current_time=updated_time;    
        }

        
        /************************************************************************/

    }
    input_file.close();
    write_output(execution);
    return 0;
}
