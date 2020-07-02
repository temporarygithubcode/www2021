#include "utility.h"
#include "graph.h"
#include "pathset.h"
#include "index.h"

int main(int argc, char* argv[])
{

    string resultFile;
    string inputFile;
    int given_k;
    string string_k;
    string indexName;
    try {
        for (int i = 1; i < argc; ++i) {
            std::string s = argv[i];
            if (s[0] == '-') {
                s = s.substr(1);

                if (s == "i") {
                    inputFile = argv[++i];
                }
                else if (s == "o") {
                    resultFile = argv[++i];
                }
                else if (s == "k"){
                    string_k=argv[++i];
                    given_k = stoi(argv[i]);
                }
                else if (s == "n"){
                    indexName = argv[++i];
                }
                else {
                    throw std::exception();
                }
            }
            else {
                throw std::exception();
            }
        }
    }
    catch (std::exception& e) {
        cout<<"error input"<<endl;
        return 1;
    }

    struct rlimit rl;
	int stacklimit = getrlimit(RLIMIT_STACK, &rl);
	rl.rlim_cur = RLIM_INFINITY;
	stacklimit = setrlimit(RLIMIT_STACK, &rl);
    struct rusage r1;
	if(getrusage(RUSAGE_SELF, &r1) != 0) {
		/*Failure*/
	}
	printf("maxrss=%ld\n", r1.ru_maxrss);



    Result result;
    string graphfile = inputFile;
    result.k = given_k;
    result.inputFile = inputFile;
    result.outputFile = "./result/"+resultFile;

    struct timespec bisimulation_startTime, bisimulation_endTime, index_startTime, index_endTime;

    OriginalGraph originalgraph = OriginalGraph();
    originalgraph.InputFile(graphfile);


    cout<<"path enumeration START "<<endl;
    clock_gettime(CLOCK_REALTIME, &bisimulation_startTime);

    PathSet path_set(given_k,originalgraph);
    clock_gettime(CLOCK_REALTIME, &bisimulation_endTime);
    long double responseTime = (bisimulation_endTime.tv_sec - bisimulation_startTime.tv_sec) + (bisimulation_endTime.tv_nsec - bisimulation_startTime.tv_nsec) * pow(10, -9);
    cout<<"path enumeration time : "<<responseTime<<endl;

    result.pathenumerationTime=responseTime;

    cout<<"indexing START "<<endl;
    clock_gettime(CLOCK_REALTIME, &index_startTime);
    PathIndex path_index(path_set);
    clock_gettime(CLOCK_REALTIME, &index_endTime);
    cout<<"indexing DONE"<<endl;
    responseTime = (index_endTime.tv_sec - index_startTime.tv_sec) + (index_endTime.tv_nsec - index_startTime.tv_nsec) * pow(10, -9);
    //cout<<responseTime<<endl;

    result.indexTime=responseTime;

    if(getrusage(RUSAGE_SELF, &r1) != 0) {
		/*Failure*/
	}
	printf("maxrss=%ld MB \n", r1.ru_maxrss/(1024));
    result.maxRSS = r1.ru_maxrss;

    result.Output();
    path_index.Output(indexName);
    return 0;
}
