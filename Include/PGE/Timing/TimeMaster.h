#ifndef PGE_TIMEMASTER_H_INCLUDED
#define PGE_TIMEMASTER_H_INCLUDED

#include "Time.h"
#include "Timing.h"

namespace PGE {

class TimeMaster {
	friend Timing;

	public:
		String print() const;

	private:
		Timer top;
		Timer* current = &top;
};

}

#endif // PGE_TIMEMASTER_H_INCLUDED