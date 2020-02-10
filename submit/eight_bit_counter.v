module eight_bit_counter(SW, KEY, HEX0, HEX1, LERD);
    input [0:0] KEY    // press to start clock?
  	input [1:0] SW // SW[0] = Clear_b, SW[1] = Enable
  	output [7:0] LEDR
  	output [6:0] HEX0; //don't undertand what this does
  	output [6:0] HEX1; //don't undertand what this does
  	
  	wire [0:6] conn;
  
  	assign T[0] = LEDR[0] & SW[1]; // does this tether the switches to the ledr's?
  	assign T[1] = LEDR[1] & T[0];
  	assign T[2] = LEDR[2] & T[1];
    assign T[3] = LEDR[3] & T[2];
    assign T[4] = LEDR[4] & T[3];
    assign T[5] = LEDR[5] & T[4];
  	assign T[6] = LEDR[6] & T[5];

  	T_ff_enable_behavior Tffeb0(.T(SW[0]), .Clk(KEY[0]), .Clear_b([SW[0]), .Q(LEDR[0]));
    T_ff_enable_behavior Tffeb1(.T(conn[0]), .Clk(KEY[0]), .Clear_b([SW[0]), .Q(LEDR[1]));
    T_ff_enable_behavior Tffeb2(.T(conn[1]), .Clk(KEY[0]), .Clear_b([SW[0]), .Q(LEDR[2]));
    T_ff_enable_behavior Tffeb3(.T(conn[2]), .Clk(KEY[0]), .Clear_b([SW[0]), .Q(LEDR[3]));
    T_ff_enable_behavior Tffeb4(.T(conn[3]), .Clk(KEY[0]), .Clear_b([SW[0]), .Q(LEDR[4]));
    T_ff_enable_behavior Tffeb5(.T(conn[4]), .Clk(KEY[0]), .Clear_b([SW[0]), .Q(LEDR[5]));
    T_ff_enable_behavior Tffeb6(.T(conn[5]), .Clk(KEY[0]), .Clear_b([SW[0]), .Q(LEDR[6]));
	T_ff_enable_behavior Tffeb7(.T(conn[6]), .Clk(KEY[0]), .Clear_b([SW[0]), .Q(LEDR[7]));
    
    hex_decoder hex0(.SW(LEDR[3:0]), .HEX(HEX0[6:0]));   
	hex_decoder hex1(.SW(LEDR[7:4]), .HEX(HEX1[6:0]));                                                                                                                           
                                                                     
endmodule
                                                                     

module T_ff_enable_behavior(input T, input Clk, input Clear_b, output reg Q);
	always @(posedge Clk, negedge Clear_b)
      if (!Clear_b) // Clear_b is low (1'b0)
		Q <= 1'b0; 
	  else if (T) 	// if Toggle is on, toggle Q to off
      	Q <= ~Q;
endmodule 


module hex_decoder(SW, HEX);
	input [3:0] SW;
	reg [6:0] result;
	output [6:0] HEX;
	always @(*)
	begin
		case (SW[3:0])
			4'b0000: result[6:0] = 7'b1000000;
			4'b0001: result[6:0] = 7'b1111001;
			4'b0010: result[6:0] = 7'b0100100;
			4'b0011: result[6:0] = 7'b0110000;
			4'b0100: result[6:0] = 7'b0011001;
			4'b0101: result[6:0] = 7'b0010010;
			4'b0110: result[6:0] = 7'b0000010;
			4'b0111: result[6:0] = 7'b1111000;
			4'b1000: result[6:0] = 7'b0000000;
			4'b1001: result[6:0] = 7'b0010000;
			4'b1010: result[6:0] = 7'b0001000;
			4'b1011: result[6:0] = 7'b0000011;
			4'b1100: result[6:0] = 7'b1000110;
			4'b1101: result[6:0] = 7'b0100001;
			4'b1110: result[6:0] = 7'b0000110;
			4'b1111: result[6:0] = 7'b0001110;
			default: result[6:0] = 7'b1000000;
		endcase
	end
	assign HEX[6:0] = result[6:0];
endmodule
