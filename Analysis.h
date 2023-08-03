#pragma once

#include "TString.h"

#include "Leafs.h"
#include "Results.h"

class Analysis {
private:
	Leafs eventData;
	Results resultData;
public:
	Analysis(TString name);
	void analyse();
	void processTask(Leafs& newEventData);
	inline Results& getResults() {return resultData;};
};

#include "Analysis.C"
