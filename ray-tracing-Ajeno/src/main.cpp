#include <chrono>

#include "utils/parser.hpp"
#include "utils/params.hpp"
#include "objects/triangle.hpp"
#include "utils/texture.hpp"
#include "includes/argh.hpp"
#include "omp.h"

int main(int, char const *argv[])
{
	std::chrono::time_point<std::chrono::system_clock>
		start, end;
	start = std::chrono::system_clock::now();

	argh::parser cmdl(argv);

	std::string path;
	cmdl({"-s", "--scene"}, "scene.json") >> path;

	int num_thread;
	int id = omp_get_max_threads();
	cmdl({"-t", "--threads"}, id) >>
		num_thread;
	if (num_thread > 0)
		omp_set_num_threads(num_thread);

	Scene scene = jsonToScene(path);

	scene.render();

	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << "Temps de calcul : " << elapsed_seconds.count()
			  << std::endl;

	return 0;
}
