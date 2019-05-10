#ifndef Wiegand_ANALYZER_SETTINGS
#define Wiegand_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class WiegandAnalyzerSettings : public AnalyzerSettings
{
public:
	WiegandAnalyzerSettings();
	virtual ~WiegandAnalyzerSettings();
	
	virtual bool SetSettingsFromInterfaces();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	void UpdateInterfacesFromSettings();

	Channel mD0Channel;
	Channel mD1Channel;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel > mD0ChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel > mD1ChannelInterface;
};

#endif //Wiegand_ANALYZER_SETTINGS
