#include "WiegandAnalyzerSettings.h"

#include <AnalyzerHelpers.h>
#include <cstring>

WiegandAnalyzerSettings::WiegandAnalyzerSettings()
:	mD0Channel( UNDEFINED_CHANNEL ),
	mD1Channel( UNDEFINED_CHANNEL )
{
	mD0ChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mD0ChannelInterface->SetTitleAndTooltip( "D0", "Data 0 Line" );
	mD0ChannelInterface->SetChannel( mD0Channel );

	mD1ChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mD1ChannelInterface->SetTitleAndTooltip( "D1", "Data 1 Line" );
	mD1ChannelInterface->SetChannel( mD1Channel );

	AddInterface( mD0ChannelInterface.get() );
	AddInterface( mD1ChannelInterface.get() );

	//AddExportOption( 0, "Export as text/csv file", "text (*.txt);;csv (*.csv)" );
	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mD0Channel, "D0", false );
	AddChannel( mD1Channel, "D1", false );
}

WiegandAnalyzerSettings::~WiegandAnalyzerSettings()
{
}

bool WiegandAnalyzerSettings::SetSettingsFromInterfaces()
{
	if( mD0ChannelInterface->GetChannel() == mD1ChannelInterface->GetChannel() )
	{
		SetErrorText( "D0 and D1 can't be assigned to the same input." );
		return false;
	}

	mD0Channel = mD0ChannelInterface->GetChannel();
	mD1Channel = mD1ChannelInterface->GetChannel();

	ClearChannels();
	AddChannel( mD0Channel, "D0", true );
	AddChannel( mD1Channel, "D1", true );

	return true;
}

void WiegandAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	const char* name_string;	//the first thing in the archive is the name of the protocol analyzer that the data belongs to.
	text_archive >> &name_string;
	if( strcmp( name_string, "SaleaeWiegandAnalyzer" ) != 0 )
		AnalyzerHelpers::Assert( "SaleaeWiegandAnalyzer: Provided with a settings string that doesn't belong to us;" );

	text_archive >> mD0Channel;
	text_archive >> mD1Channel;

	ClearChannels();
	AddChannel( mD0Channel, "D0", true );
	AddChannel( mD1Channel, "D1", true );

	UpdateInterfacesFromSettings();
}

const char* WiegandAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << "SaleaeWiegandAnalyzer";
	text_archive << mD0Channel;
	text_archive << mD1Channel;

	return SetReturnString( text_archive.GetString() );
}

void WiegandAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mD0ChannelInterface->SetChannel( mD0Channel );
	mD1ChannelInterface->SetChannel( mD1Channel );
}
