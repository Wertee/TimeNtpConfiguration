
#include "TimeWidget.h"

#include "EditTimeWidget.h"
#include "NtpWidget.h"
#include "ShowTimeWidget.h"
#include "../StatusCommands/NtpStatusCommandsWidget.h"

namespace fs = std::filesystem;

TimeWidget::TimeWidget()
{
    show_time_widget_ = this->addWidget(std::make_unique<ShowTimeWidget>());
    edit_time_widget_ = this->addWidget(std::make_unique<EditTimeWidget>());
    ntp_widget_ = this->addWidget(std::make_unique<NtpWidget>());
    ntp_widget_->ntpStateChanged().connect(
        this, [this](bool state) { edit_time_widget_->set_edit_access(!state); });

    auto row_container = this->addWidget(std::make_unique<Wt::WContainerWidget>());
    row_container->setStyleClass("row justify-content-start m-2");

    auto button_save_configuration = this->addWidget(std::make_unique<Wt::WPushButton>("Сохранить выбранные настройки"));
    button_save_configuration->setStyleClass("btn btn-primary m-2");
    button_save_configuration->clicked().connect([this]{save_configuration();});

    load_configuration();
}

bool TimeWidget::check_is_ntp_off() {
    const int attempts = 10;
    const int delay_ms = 500;

    for (int i = 0; i < attempts; ++i) {
        int ntp_status = std::system("timedatectl show -p NTP | grep -q 'NTP=no'");
        if (ntp_status == 0) {
            return true;  // NTP отключен
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    return false;
}

void TimeWidget::save_configuration()
{
    if (!ntp_widget_->get_ntp_checkbox_value()) {
        std::string command_set_time = "timedatectl set-time \"" + edit_time_widget_->get_date() + " " + edit_time_widget_->get_time()+ "\"";

        int result_set_ntp_off = std::system("timedatectl set-ntp false");
        if (result_set_ntp_off != 0) {
            Wt::WMessageBox::show("Ошибка!", "Ошибка при выполнении команды отключения NTP", Wt::StandardButton::Ok);
            return;
        }

        if (!check_is_ntp_off()) {
            Wt::WMessageBox::show("Ошибка!", "NTP не отключился после выполнения команды!", Wt::StandardButton::Ok);
            return;
        }

        int result_set_time = std::system(command_set_time.c_str());
        if (result_set_time != 0) {
            Wt::WMessageBox::show("Ошибка!", "Не удалось изменить время", Wt::StandardButton::Ok);
            return;
        }
        Wt::WMessageBox::show("Успех!", "NTP отключен, время установлено вручную", Wt::StandardButton::Ok);
    }else {
        int result_set_ntp = std::system("timedatectl set-ntp true");
        if (result_set_ntp != 0) {
            Wt::WMessageBox::show("Ошибка!", "Ошибка при включении NTP", Wt::StandardButton::Ok);
            return;
        }

        int result_updating_file = update_systemd_timesyncd_conf(ntp_widget_->get_main_servers(),ntp_widget_->get_reserve_servers());

        int result_set_timezone(set_timezone(ntp_widget_->get_timezone()));
        if (result_set_timezone != 0) {
            return;
        }

        if (result_updating_file == 0) {
            int result_restart = std::system("systemctl restart systemd-timesyncd");
            if (result_restart != 0) {
                Wt::WMessageBox::show("Ошибка!", "Ошибка при перезапуске timesyncd", Wt::StandardButton::Ok);
            }
        }
        Wt::WMessageBox::show("Успех!", "Настройки успешно применены", Wt::StandardButton::Ok);
    }
    if (ntp_widget_->get_ntp_checkbox_value())
        create_configuration_file(true, "", "", ntp_widget_->get_main_servers(), ntp_widget_->get_reserve_servers(),ntp_widget_->get_timezone());
    else
        create_configuration_file(false, edit_time_widget_->get_date(), edit_time_widget_->get_time(), "", "", "");
}

void TimeWidget::create_configuration_file(bool ntp_on,const std::string& date, const std::string& time,std::string main_servers,std::string reserve_servers,const std::string& timezone)
{
    std::ofstream conf(LOCAL_CONFIGURATION_FILE);
    if (!conf.is_open())
    {
        Wt::WMessageBox::show("Ошибка!", "Ошибка при создании файла конфигурации ", Wt::StandardButton::Ok);
        return;
    }
    conf << "ntp_on=" << (ntp_on ? "1":"0") << std::endl;
    conf << "Date="<< date << std::endl;
    conf << "Time="<< time << std::endl;
    conf << "NTP="<< main_servers << std::endl;
    conf << "FallbackNTP="<< reserve_servers << std::endl;
    conf << "Timezone=" << timezone << std::endl;

    conf.close();
}


void TimeWidget::load_configuration() {

    fs::path file_path = LOCAL_CONFIGURATION_FILE;

    if (!exists(file_path)) {
        create_configuration_file(false,Wt::WDate::currentServerDate().toString("yyyy-MM-dd").toUTF8(),Wt::WTime::currentServerTime().toString("HH:mm:ss").toUTF8(),"","","");
    }

    std::ifstream file(file_path);
    std::string line;
    if (!file.is_open())
        Wt::WMessageBox::show("Ошибка!", "Ошибка при открытии файла конфигурации ", Wt::StandardButton::Ok);
    bool ntp_on = false;
    bool can_search_file = false;
    ntp_widget_->clear_current_ntp_servers();
    while (getline(file, line)) {
        if (line.find("ntp_on=") != std::string::npos) {
            ntp_on = line.substr(line.find("ntp_on=") + 7) == "1";
            ntp_widget_->set_ntp_checkbox_value(ntp_on);
            edit_time_widget_->set_edit_access(!ntp_on);
            can_search_file = true;
        }
        if (!can_search_file) {
            file.close();
            break;
        }

        if (ntp_on) {
            if (line.find("NTP=") == 0) {
                ntp_widget_->set_ntp_servers(line.substr(line.find("NTP=") + 4));
            }
            if (line.find("FallbackNTP=") == 0) {
                ntp_widget_->set_ntp_servers(line.substr(line.find("FallbackNTP=") + 12),true);
            }
            if (line.find("Timezone=") == 0) {
                ntp_widget_->set_timezone(line.substr(line.find("Timezone=") + 9));
            }
        } else {
            if (line.find("Date=") == 0) {
                edit_time_widget_->set_date_from_string(line.substr(line.find("Date=") + 5));
            }
            if (line.find("Time=") == 0) {
                edit_time_widget_->set_time_from_string(line.substr(line.find("Time=") + 5));
            }
        }
    }
}

int TimeWidget::set_timezone(const std::string& timezone)
{
    std::string command = "timedatectl set-timezone " + timezone;
    int result = std::system(command.c_str());

    if (result == 0) {
        return 0;
    }
    Wt::WMessageBox::show("Ошибка", "Ошибка при установке временной зоны", Wt::StandardButton::Ok);
    return 1;
}

int TimeWidget::update_systemd_timesyncd_conf(std::string main_servers, std::string fallback_servers)
{
    const std::string temp_filename = std::string(SYSTEMD_CONF_FILE) + ".tmp";

    std::ifstream file(SYSTEMD_CONF_FILE);
    if (!file.is_open()) {
        Wt::WMessageBox::show("Ошибка!", "Ошибка при открытии файла timesyncd.conf", Wt::StandardButton::Ok);
        return 1;
    }

    std::ofstream temp_file(temp_filename);
    if (!temp_file.is_open()) {
        Wt::WMessageBox::show("Ошибка!", "Ошибка при создании временного файла", Wt::StandardButton::Ok);
        return 1;
    }
    bool write_error = false;
    std::string line;
    while (getline(file,line))
    {
        if (line.find("#NTP=") == 0 || line.find("NTP=") == 0) {
            line = "NTP=" + main_servers;
        } else if (line.find("#FallbackNTP=") == 0 || line.find("FallbackNTP=") == 0) {
            line = "FallbackNTP=" + fallback_servers;
        }
        if (!(temp_file << line << std::endl)) {
            Wt::WMessageBox::show("Ошибка!", "Ошибка записи во временный файл", Wt::StandardButton::Ok);
            write_error = true;
            break;
        }
    }

    file.close();
    temp_file.close();

    if (write_error)
    {
        std::filesystem::remove(temp_filename);
        return 1;
    }

    if (rename(temp_filename.c_str(), SYSTEMD_CONF_FILE) != 0) {
        Wt::WMessageBox::show("Ошибка!", "Ошибка замены файла", Wt::StandardButton::Ok);
        remove(temp_filename.c_str());
        return 1;
    }
    return 0;
}
