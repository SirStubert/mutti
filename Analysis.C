#include "Analysis.h"

Analysis::Analysis(TString name) {
	resultData.setNewName(name);
}

void Analysis::analyse() {
	resultData.fillHist(eventData.getValue());
}

void Analysis::processTask(Leafs& newTask) {
	eventData = newTask;
	analyse();
}
