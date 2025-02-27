#ifndef NTPWIDGET_H
#define NTPWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WCheckBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>
#include <Wt/WComboBox.h>
#include <Wt/WMessageBox.h>
#include <Wt/WTable.h>
#include <Wt/WRegExpValidator.h>
#include <Wt/WValidator.h>
#include <Wt/WTableRow.h>
#include<fstream>
#include <boost/algorithm/string.hpp>

class NtpWidget: public Wt::WContainerWidget {
public:
    NtpWidget();
    Wt::Signal<bool>& ntpStateChanged() { return ntp_checkbox_changed_; }
    std::string get_main_servers();
    std::string get_reserve_servers();
    bool get_ntp_checkbox_value();
    std::string get_timezone();
    void set_ntp_checkbox_value(bool value);
    void set_ntp_servers(std::string value, bool reserve = false);
    void set_timezone(std::string timezone);
    void clear_current_ntp_servers();
    bool ntp_server_is_valid(const std::shared_ptr<Wt::WValidator>& validator);
private:
    Wt::Signal<bool> ntp_checkbox_changed_;
    Wt::WCheckBox* ntp_check_box_;
    std::set<int> selectedRows;
    Wt::WContainerWidget* address_container_;
    Wt::WLineEdit* ntp_server_;
    Wt::WCheckBox* reserve_ntp_server_;
    Wt::WComboBox* timezone_cb_;
    Wt::WTable* table_current_ntp_servers_;
    std::vector<std::pair<std::string,bool>> current_ntp_servers_;
    WContainerWidget* ntp_container_;
    void update_table_current_ntp_servers();
    void add_ntp_server();
    void delete_ntp_server();
    void set_timezone();
    int get_current_timezone_index();
    void set_access_by_ntp_checkbox(WContainerWidget* cw);

    const std::vector<std::pair<std::string, std::string>> timezones = {
        {"Калининградское время (UTC+2)", "Europe/Kaliningrad"},
        {"Московское время (UTC+3)", "Europe/Moscow"},
        {"Самарское время (UTC+4)", "Europe/Samara"},
        {"Екатеринбургское время (UTC+5)", "Asia/Yekaterinburg"},
        {"Омское время (UTC+6)", "Asia/Omsk"},
        {"Новосибирское время (UTC+7)", "Asia/Novosibirsk"},
        {"Красноярское время (UTC+7)", "Asia/Krasnoyarsk"},
        {"Иркутское время (UTC+8)", "Asia/Irkutsk"},
        {"Читинское время (UTC+9)", "Asia/Chita"},
        {"Якутское время (UTC+9)", "Asia/Yakutsk"},
        {"Владивостокское время (UTC+10)", "Asia/Vladivostok"},
        {"Магаданское время (UTC+11)", "Asia/Magadan"},
        {"Сахалинское время (UTC+11)", "Asia/Sakhalin"},
        {"Камчатское время (UTC+12)", "Asia/Kamchatka"},
        {"Анадырское время (UTC+12)", "Asia/Anadyr"}
    };
};

#endif //NTPWIDGET_H