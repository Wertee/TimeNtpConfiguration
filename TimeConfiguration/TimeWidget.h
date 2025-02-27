#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WFileUpload.h>
#include <Wt/WProgressBar.h>
#include <fstream>
#include <filesystem>
#include <thread>

#include "ShowTimeWidget.h"
#include "EditTimeWidget.h"
#include "NtpWidget.h"

#define LOCAL_CONFIGURATION_FILE "localsync.conf"
#define SYSTEMD_CONF_FILE "/etc/systemd/timesyncd.conf"

class TimeWidget: public Wt::WContainerWidget
{
public:
    TimeWidget();
private:
    ShowTimeWidget* show_time_widget_;
    EditTimeWidget* edit_time_widget_;
    NtpWidget* ntp_widget_;
    void save_configuration();
    void load_configuration();
    void create_configuration_file(bool ntp_on,const std::string& date, const std::string& time,std::string main_servers,std::string reserve_servers,const std::string& timezone);
    int update_systemd_timesyncd_conf(std::string main_servers,std::string fallback_servers);
    bool check_is_ntp_off();
    int set_timezone(const std::string& timezone);
};

#endif //TIMEWIDGET_H
