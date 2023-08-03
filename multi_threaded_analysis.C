#include "TROOT.h"
#include "TStopwatch.h"
#include "TString.h"

#include <iostream>
#include <thread>
#include <mutex>

#include "Leafs.h"
#include "Analysis.h"
#include "Results.h"

// Mutex locks to prevent multiple threads from manipulating the same variable at the same time
std::mutex printMutex;
std::mutex analysisMutex;
std::mutex mergeMutex;

// Some global variables for simplicity
Leafs eventData;
Results resultData("results");
long long taskID = 0;

TTree* createTree() {
	TTree* tree = new TTree("tree", "tree");
	tree->Branch("branch", eventData.getValueAddress());
	return tree;
}

void fillTree(TTree* tree, long long nEntries) {
	for (long long i = 0; i < nEntries; i++) {
		eventData.setValue(gRandom->Gaus(5., 1.));
		tree->Fill();
	}
}

void printThreadStatus(int threadID, TString& string) {
	while (!(printMutex.try_lock())) {
		std::cout << "thread " << threadID << ": " << string << "\n";
		printMutex.unlock();
	}
}

void setNewTask(TTree* tree, long long taskID, Leafs& task) {
	tree->GetEntry(taskID);
	task = eventData;
}

bool getTask(TTree* tree, Leafs& task, long long nTasks) {
	bool isNewTask = false;
	while (!(analysisMutex.try_lock())) {
		isNewTask = taskID < nTasks ? true : false;
		if (isNewTask)
			setNewTask(tree, taskID++, task);
		analysisMutex.unlock();
	}
	return isNewTask;
}

void mergeResults(Results& newResults) {
	while (!(mergeMutex.try_lock())) {
		resultData.mergeResults(newResults);
                mergeMutex.unlock();
        }
}

void threadStarter(int threadID, TTree* tree, long long nTasks) {
	TString status = "started";
	printThreadStatus(threadID, status);
	TString name = "thread_" + TString::Itoa(threadID, 10);
	Analysis dataAnalyzer(name);
	Leafs task;
	long long taskID = 0;
	while (getTask(tree, task, nTasks))
		dataAnalyzer.processTask(task);
	mergeResults(dataAnalyzer.getResults());
	status = "ended";
	printThreadStatus(threadID, status);
}

void initializeThreads(std::vector<std::thread>& threads, int nThreads, TTree* tree) {
	long long nEntries = tree->GetEntries();
	for (int i = 0; i < nThreads; i++)
		threads.push_back(std::thread(threadStarter, i, tree, nEntries));
	return threads;
}

void closeThreads(std::vector<std::thread>& threads) {
	for (size_t i = 0; i < threads.size(); i++) {
		threads.at(i).join();
		TString status = "closed";
		printThreadStatus(i, status);
	}
}

void multiThreadedAnalysis(TTree* tree) {
	std::vector<std::thread> threads;
	initializeThreads(threads, 4, tree);
	closeThreads(threads);
}

void drawResults() {
	TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
	canvas->cd(1);
	resultData.getHist()->Draw();
}

int multi_threaded_analysis() {
	TTree* tree = createTree();
	fillTree(tree, 1000);
	multiThreadedAnalysis(tree);
	drawResults();
	return 0;
}
