#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env){
    int i;
    int clk;
    int pause_remaining = 0;
    bool just_paused = false;

    Verilated::commandArgs(argc, argv);
    //init top verilog instance
    Vcounter* top = new Vcounter;
    //init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp,99);
    tfp->open("counter.vcd");

    //initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    
    // run simulation for many clock cycles
    // counter pauses at 9 for 3 cycles then continues
    for (i=0; i<300; i++){
        /* if (pause_remaining>0 || (top->count == 9 &&!just_paused)){
            if (pause_remaining == 0) pause_remaining = 3;           
            top->en = 0;
            pause_remaining--;
            if (pause_remaining == 0) just_paused = true;
        }       
        else{
            top->en = (i>4);
            just_paused = false;
        } */

        // dump variables into VCD file and toggle clock
        for (clk=0;clk<2;clk++){
            tfp->dump (2*i+clk); //unit = ps
            top->clk = !top->clk;
            top->eval ();
        }

        top->rst = (i<2|i==15); //remove or i ==15 for pausing at 9 to continue at 10
        top ->en = (i>4); //comment out this line for pausing at 9
        if (Verilated::gotFinish()) exit(0);
    }
        
    tfp->close();
    exit(0);

}



// cd /mnt/c/Users/tina_/Desktop/Lab1-Counter/task1 gtkwave counter.vcd