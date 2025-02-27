#ifndef NTPCONFIG_H
#define NTPCONFIG_H
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <string>
#include <array>

class NtpStatusCommandsWidget: public Wt::WContainerWidget {
public:
    NtpStatusCommandsWidget();
    void update_content();
private:
    std::vector<std::string> command_result_;
    void executeCommand(const std::string& command);
    WContainerWidget* ntp_config_main_widget_;
};

#endif //NTPCONFIG_H
