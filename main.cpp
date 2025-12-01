#include<iostream>
#include"board.h"

int main(){
    Board board;
    ul someint = board.b_board | board.w_board;
    std::cout << someint << '\n';

 	ul a = 12;
	std::cout << a << '\n';
    return 0;
}
