#ifndef SERIAL_SIMULATION_DATA_GENERATOR
#define SERIAL_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>
#include "WiegandAnalyzerSettings.h"
#include <stdlib.h>

class WiegandSimulationDataGenerator
{
public:
	WiegandSimulationDataGenerator();
	~WiegandSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, WiegandAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );

protected:
	WiegandAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;
	U8 mValue;

protected:	//Wiegand specific
			//functions
	void CreateWiegandOutput( U64 number );
	void CreateBit( BitState bit_state );

protected: //vars
	ClockGenerator mClockGenerator;

	SimulationChannelDescriptorGroup mWiegandSimulationChannels;
	SimulationChannelDescriptor* mD0;
	SimulationChannelDescriptor* mD1;
};
#endif //UNIO_SIMULATION_DATA_GENERATOR
