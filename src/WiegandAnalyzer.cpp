#include "WiegandAnalyzer.h"
#include "WiegandAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

WiegandAnalyzer::WiegandAnalyzer()
:	Analyzer2(),  
	mSettings( new WiegandAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

WiegandAnalyzer::~WiegandAnalyzer()
{
	KillThread();
}

void WiegandAnalyzer::SetupResults()
{
	mResults.reset( new WiegandAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mD0Channel );
}

void WiegandAnalyzer::WorkerThread()
{
	mD0 = GetAnalyzerChannelData( mSettings->mD0Channel );
	mD1 = GetAnalyzerChannelData( mSettings->mD1Channel );

	bool d0Falling = false;
	bool d1Falling = false;
	U64 firstBit = 0;
	U64 lastBit = 0;
	auto d0Pos = mD0->GetSampleNumber();
	auto d1Pos = mD1->GetSampleNumber();

	U64 result = 0;

	for( ; ; )
	{
		// Advance to falling edges only
		while (!d0Falling) {
			if (!mD0->DoMoreTransitionsExistInCurrentData()) {
				d0Pos = -1;
				break;
			}

			mD0->AdvanceToNextEdge();
			if (mD0->GetBitState() == BIT_LOW) {
				d0Falling = true;
				d0Pos = mD0->GetSampleNumber();
				break;
			}

		}

		while (!d1Falling) {
			if (!mD1->DoMoreTransitionsExistInCurrentData()) {
				d1Pos = -1;
				break;
			}

			mD1->AdvanceToNextEdge();
			if (mD1->GetBitState() == BIT_LOW) {
				d1Falling = true;
				d1Pos = mD1->GetSampleNumber();
				break;
			}

		}

		// Put a marker on the spot we are at.
		if (d0Pos < d1Pos && d0Falling) {
			if (!firstBit)
				firstBit = d0Pos;

			lastBit = d0Pos;
			mResults->AddMarker(d0Pos, AnalyzerResults::Zero, mSettings->mD0Channel);
			d0Falling = false;
			result <<= 1;
		}

		if (d1Pos < d0Pos && d1Falling) {
			if (!firstBit)
				firstBit = d1Pos;

			lastBit = d1Pos;
			mResults->AddMarker(d1Pos, AnalyzerResults::One, mSettings->mD1Channel);
			d1Falling = false;
			result <<= 1;
			result |= 1;
		}

		if (! mD0->DoMoreTransitionsExistInCurrentData() && ! mD1->DoMoreTransitionsExistInCurrentData()) {
			Frame frame;
			frame.mStartingSampleInclusive = firstBit;
			frame.mEndingSampleInclusive = lastBit;
			frame.mData1 = result;
			mResults->AddFrame(frame);
			mResults->CommitResults();
			break;
		}

		CheckIfThreadShouldExit();
	}
}

bool WiegandAnalyzer::NeedsRerun()
{
	return false;
}

U32 WiegandAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 WiegandAnalyzer::GetMinimumSampleRateHz()
{
	return 2000000;
}

const char* WiegandAnalyzer::GetAnalyzerName() const
{
	return "Wiegand";
}

const char* GetAnalyzerName()
{
	return "Wiegand";
}

Analyzer* CreateAnalyzer()
{
	return new WiegandAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
