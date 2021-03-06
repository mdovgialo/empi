/**********************************************************
 * University of Warsaw, Department of Biomedical Physics *
 *   Enhanced Matching Pursuit Implementation (empi)      *
 * See README.md and LICENCE for details.                 *
 **********************************************************/
#ifndef EMPI_GABOR_HPP
#define	EMPI_GABOR_HPP

#include <memory>
#include "classes.hpp"
#include "fftw.hpp"

struct GaborProductEstimator {
	const double s12, s22, A, part;

	GaborProductEstimator(double s1, double s2);

	double estimate(double t1, double t2) const;
};

class GaborWorkspaceMap : public TimeFreqMap<complex> {
	const int Nfft;
	const double freqSampling;
	fftwDouble input;
	fftwComplex output;
	fftwPlan plan;

	GaborWorkspaceMap(const GaborWorkspaceMap&); // forbidden
	void operator=(const GaborWorkspaceMap&); // forbidden

public:
	const double s;
	const size_t atomCount;

	GaborWorkspaceMap(double s, int fCount, int tCount, double freqSampling, double tMax, int channelCount);

	void compute(const MultiSignal& signal);

	void compute(const SingleSignal& signal, int cIndex, int tIndex);
};

class GaborWorkspace : public Workspace {
	const double freqSampling;
	std::vector< std::shared_ptr<GaborWorkspaceMap> > maps;

public:
	static void subtractAtomFromSignal(Atom& atom, SingleSignal& signal, bool fit);

	GaborWorkspace(double freqSampling, std::vector< std::shared_ptr<GaborWorkspaceMap> >&& maps)
	: freqSampling(freqSampling), maps(maps) { }

	Atoms findBestMatch(MultichannelConstraint constraint = nullptr) const;

	size_t getAtomCount(void) const;

	void subtractAtom(const Atom& atom, SingleSignal& signal, int channel);
};

class GaborWorkspaceBuilder : public WorkspaceBuilder {
	double energyError;

public:
	GaborWorkspaceBuilder(double energyError)
	: energyError(energyError) { }

	Workspace* buildWorkspace(const MultiSignal& signal) const;
};

#endif	/* EMPI_GABOR_HPP */
