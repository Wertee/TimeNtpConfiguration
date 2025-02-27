
#include "NtpWidget.h"

NtpWidget::NtpWidget()
{
    ntp_check_box_ = this->addWidget(std::make_unique<Wt::WCheckBox>("Включить NTP"));
    ntp_check_box_->setUnChecked();
    ntp_check_box_->setStyleClass("m-2");

    ntp_container_ = this->addWidget(std::make_unique<WContainerWidget>());

    set_access_by_ntp_checkbox(ntp_container_);
    ntp_check_box_->changed().connect([this]
    {
       ntp_checkbox_changed_.emit(ntp_check_box_->isChecked());
       set_access_by_ntp_checkbox(ntp_container_);
    });

    address_container_ = ntp_container_->addWidget(std::make_unique<WContainerWidget>());
    address_container_->addStyleClass("input-group w-50");

    auto textAddress =address_container_->addWidget(std::make_unique<Wt::WText>("Адрес сервера:"));
    textAddress->setStyleClass("m-2");

    ntp_server_ = address_container_->addWidget(std::make_unique<Wt::WLineEdit>());
    ntp_server_->setToolTip("Укажите адрес сервера в формате ntp.ubuntu.com");
    ntp_server_->addStyleClass("form-control m-2");

    //Сгенерировано нейросетью. Я уже и не помню, когда последний раз имел дело с regex
    std::string regexPattern = R"((?:[a-zA-Z0-9-]+\.)+[a-zA-Z]{2,})";
    auto address_validator = std::make_shared<Wt::WRegExpValidator>(regexPattern);
    ntp_server_->setValidator(address_validator);

    reserve_ntp_server_ = address_container_->addWidget(std::make_unique<Wt::WCheckBox>("Резервный"));
    reserve_ntp_server_->setUnChecked();
    reserve_ntp_server_->addStyleClass("m-2");


    auto button_add_server = ntp_container_->addWidget(std::make_unique<Wt::WPushButton>("Добавить NTP сервер"));
    button_add_server->setStyleClass("btn btn-primary m-2");
    button_add_server->clicked().connect([this]{add_ntp_server();});

    table_current_ntp_servers_ = ntp_container_->addWidget(std::make_unique<Wt::WTable>());
    table_current_ntp_servers_->setHeaderCount(1);

    table_current_ntp_servers_->elementAt(0,0)->addNew<Wt::WText>("");
    table_current_ntp_servers_->elementAt(0,1)->addNew<Wt::WText>("№");
    table_current_ntp_servers_->elementAt(0,2)->addNew<Wt::WText>("Адрес");
    table_current_ntp_servers_->elementAt(0,3)->addNew<Wt::WText>("Резервный");

    table_current_ntp_servers_->addStyleClass("table table-bordered table-striped m-2 w-50");
    update_table_current_ntp_servers();

    auto button_delete_server = ntp_container_->addWidget(std::make_unique<Wt::WPushButton>("Удалить сервер"));
    button_delete_server->setStyleClass("btn btn-danger m-2");
    button_delete_server->clicked().connect([this]{delete_ntp_server();});

    timezone_cb_ = ntp_container_->addWidget(std::make_unique<Wt::WComboBox>());
    timezone_cb_->setStyleClass("form-select m-2 w-50");
    for (const auto& tzname : timezones)
    {
        timezone_cb_->addItem(tzname.first);
    }
}

bool NtpWidget::ntp_server_is_valid(const std::shared_ptr<Wt::WValidator>& validator) {
    auto result_validation = validator->validate(ntp_server_->text());
    if (result_validation.state() != Wt::ValidationState::Valid) {
        return false;
    }
    return true;
}

void NtpWidget::update_table_current_ntp_servers()
{
    if (current_ntp_servers_.empty())
        return;

    for (int i = table_current_ntp_servers_->rowCount()-1; i > 0 ; --i) {
        table_current_ntp_servers_->removeRow(i);
    }

    for (int i = 0; i < current_ntp_servers_.size(); ++i)
    {
        int row = table_current_ntp_servers_->rowCount();
        table_current_ntp_servers_->elementAt(row,0)->addNew<Wt::WCheckBox>();
        table_current_ntp_servers_->elementAt(row,1)->addNew<Wt::WText>(Wt::WString("{1}").arg(row));
        table_current_ntp_servers_->elementAt(row,2)->addNew<Wt::WText>(current_ntp_servers_[i].first);
        table_current_ntp_servers_->elementAt(row,3)->addNew<Wt::WText>(current_ntp_servers_[i].second?"Да":"");
    }
}

void NtpWidget::set_access_by_ntp_checkbox(WContainerWidget* cw) {
    if (ntp_check_box_->isChecked())
        cw->setDisabled(false);
    else
        cw->setDisabled(true);
}

void NtpWidget::add_ntp_server()
{
    if (ntp_server_->text().empty())
        return;

    if (!ntp_server_is_valid(ntp_server_->validator())) {
        Wt::WMessageBox::show("Ошибка!", "Вы ввели некорректный адрес NTP сервера", Wt::StandardButton::Ok);
        ntp_server_->setText("");
        return;
    }

    for (auto server : current_ntp_servers_)
    {
        if (server.first == ntp_server_->text())
        {
            Wt::WMessageBox::show("Ошибка!", "Такой сервер уже добавлен", Wt::StandardButton::Ok);
            ntp_server_->setText("");
            return;
        }
    }
    current_ntp_servers_.push_back(std::pair(ntp_server_->text().toUTF8(),reserve_ntp_server_->isChecked()));
    update_table_current_ntp_servers();
    ntp_server_->setText("");
}

void NtpWidget::delete_ntp_server()
{
    for (int row = table_current_ntp_servers_->rowCount() - 1; row >= 0; --row) {
        auto *checkbox = dynamic_cast<Wt::WCheckBox*>(table_current_ntp_servers_->elementAt(row, 0)->widget(0));
        if (checkbox && checkbox->isChecked()) {
            auto *textWidget = dynamic_cast<Wt::WText*>(table_current_ntp_servers_->elementAt(row, 2)->widget(0));
            if (textWidget) {
                std::string serverName = textWidget->text().toUTF8();

                auto it = std::remove_if(current_ntp_servers_.begin(), current_ntp_servers_.end(),
                                         [&serverName](const std::pair<std::string, bool> &entry) {
                                             return entry.first == serverName;
                                         });
                if (it != current_ntp_servers_.end()) {
                    current_ntp_servers_.erase(it, current_ntp_servers_.end());
                }
            }
            table_current_ntp_servers_->removeRow(row);
        }
    }
    update_table_current_ntp_servers();
}

std::string NtpWidget::get_timezone()
{
    auto selected_timezone = timezone_cb_->currentIndex();
    return timezones.at(selected_timezone).second;
}


bool NtpWidget::get_ntp_checkbox_value()
{
    return ntp_check_box_->isChecked();
}

std::string NtpWidget::get_main_servers()
{
    std::string res;
    for (auto server : current_ntp_servers_)
    {
        if (!server.second)
            res += " " + server.first;
    }
    boost::trim(res);
    return res;
}

std::string NtpWidget::get_reserve_servers()
{
    std::string res = "";
    for (auto server : current_ntp_servers_)
    {
        if (server.second)
            res += " " + server.first;
    }
    boost::trim(res);
    return res;
}

void NtpWidget::set_ntp_checkbox_value(bool value) {
    ntp_check_box_->setChecked(value);
    set_access_by_ntp_checkbox(ntp_container_);
}

void NtpWidget::set_timezone(std::string timezone) {
    for (int i = 0; i < timezones.size(); ++i) {
        if (timezones[i].second == timezone)
            timezone_cb_->setCurrentIndex(i);
    }
}

void NtpWidget::set_ntp_servers(std::string value, bool reserve) {
    std::vector<std::string> res;
    split(res, value, boost::is_any_of(" "));
    for (auto re: res) {
        current_ntp_servers_.push_back(std::make_pair(re,reserve));
    }
    update_table_current_ntp_servers();
}

void NtpWidget::clear_current_ntp_servers() {
    current_ntp_servers_.clear();
}
