#include "Results.h"

Results::Results() {

}

Results::Results(TString newName) {
	setNewName(newName);
}

void Results::setNewName(TString newName) {
	name = newName;
	hist = new TH1D("hist" + name, "NAME;x (a.u.);y (a.u.)", 1000, 0., 10.);
}

void Results::mergeResults(Results& otherResults) {
	hist->Add(otherResults.getHist());
}
