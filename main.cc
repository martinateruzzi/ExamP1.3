#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <map>

#include "bst.hh"

int main(int argc, char*argv[]) {

	try {
		
		if(argc!=2) throw std::runtime_error("Please, give the number N of nodes in the tree as argument.");
		
		bst<int,int> tree{};
		std::map<int,int> map{};

		std::srand(std::time(nullptr));

		int trial = std::atoi(argv[1]);

		for(int i=0; i<trial; ++i){
			int j = std::rand();
			tree.emplace(j,j);
			map.emplace(j,j);	
		}

		for(int i=0; i<11; ++i){
			auto t0 = std::chrono::high_resolution_clock::now();
			tree.find(i);
			auto t1 = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0);
			std::cout << "Elapsed " << elapsed.count() << " [nanoseconds]" << std::endl;
		}


		tree.balance();

		std::cout << std::endl << std::endl;

		for(int i=0; i<11; ++i){
			auto t0 = std::chrono::high_resolution_clock::now();
			map.find(i);
			auto t1 = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0);
			std::cout << "Elapsed " << elapsed.count() << " [nanoseconds]" << std::endl;
		}	

		std::cout << std::endl << std::endl;

		for(int i=0; i<11; ++i){
			auto t0 = std::chrono::high_resolution_clock::now();
			tree.find(i);
			auto t1 = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0);
			std::cout << "Elapsed " << elapsed.count() << " [nanoseconds]" << std::endl;
		}

		tree.clear();

	return 0;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
	}

  return 0;
}
