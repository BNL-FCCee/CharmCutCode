#ifndef _COMPUTECOMPATIBILITY_H
#define _COMPUTECOMPATIBILITY_H

#include "scanBase.h"

#include <string>
#include <iostream>

class computeCompatibility: public scanBase
{
   	public:
   		computeCompatibility(std::map<TString, TString>& opts);
   		~computeCompatibility();

   	protected:

      	void parseScanOpt() override {return ;};
  		void runScan() override;
  		void scanTypeSetup() override;

};


#endif


