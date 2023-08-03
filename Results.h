#pragma once

#include "TH1D.h"
#include "TString.h"

class Results {
private:
	TString name = "";
        TH1D* hist = nullptr;

public:
	Results();
	Results(TString newName);
	void setNewName(TString newName);
	inline TH1D* getHist() {return hist;};
	inline void fillHist(double value) {hist->Fill(value);};
	void mergeResults(Results& otherResults);
};

#include "Results.C"
