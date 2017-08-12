// create module
module blink(clk, LED);
input clk;     // input clock 50MHz
output LED;

// create a binary counter
reg [31:0] cnt; //32 bit counter
initial cnt <= 0; // start count at zero 
always @(posedge clk) cnt <= cnt+1; // count up

//assign LED to nth bit of the counter
assign LED = cnt[26]; // Use the 25th bit of the counter to blink the LED at a few Hz
endmodule