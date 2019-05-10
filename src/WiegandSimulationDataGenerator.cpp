#include "WiegandSimulationDataGenerator.h"


WiegandSimulationDataGenerator::WiegandSimulationDataGenerator()
{
}

WiegandSimulationDataGenerator::~WiegandSimulationDataGenerator()
{
}

void WiegandSimulationDataGenerator::Initialize( U32 simulation_sample_rate, WiegandAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mClockGenerator.Init( 400000, simulation_sample_rate );

	mD0 = mWiegandSimulationChannels.Add( settings->mD0Channel, mSimulationSampleRateHz, BIT_HIGH );
	mD1 = mWiegandSimulationChannels.Add( settings->mD1Channel, mSimulationSampleRateHz, BIT_HIGH );

	mWiegandSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 10.0 ) ); //insert 10 bit-periods of idle

	mValue = 0;
}

U32 WiegandSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	// FIXME - this does not really work well.
	while( mD1->GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
		mWiegandSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 500 ) );
		if( rand() % 20 == 0 ){
			mWiegandSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 80 ) );
		}
		

		CreateWiegandOutput(0xDEADBEEF);
		mWiegandSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 80 ) );
	}

	*simulation_channels = mWiegandSimulationChannels.GetArray();
	return mWiegandSimulationChannels.GetCount();
}



void WiegandSimulationDataGenerator::CreateWiegandOutput(U64 data)
{
	for (int i=0; i < 32; i++) {
		BitState b = BIT_LOW;
		if (data & 0x80000000)
			b = BIT_HIGH;

		CreateBit(b);
		data <<= 1;
	}
}

void WiegandSimulationDataGenerator::CreateBit( BitState bit_state )
{
	if (mD0->GetCurrentBitState() != BIT_HIGH || mD1->GetCurrentBitState() != BIT_HIGH)
		AnalyzerHelpers::Assert("CreateBit expects to be entered with d0 and d1 high");

	mWiegandSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 0.5 ) );

	SimulationChannelDescriptor* tbit = mD0;
	if (bit_state == BIT_HIGH)
		tbit = mD1;

	tbit->Transition();
	mWiegandSimulationChannels.AdvanceAll( mClockGenerator.AdvanceByHalfPeriod( 0.5 ) );
	tbit->Transition();
}
