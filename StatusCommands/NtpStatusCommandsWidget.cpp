#include "NtpStatusCommandsWidget.h"

NtpStatusCommandsWidget::NtpStatusCommandsWidget() {
    ntp_config_main_widget_ = this->addWidget(std::make_unique<WContainerWidget>());
    update_content();
}

void NtpStatusCommandsWidget::executeCommand(const std::string& command)
{
    command_result_.clear();
    std::array<char, 128> buffer;
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), [](FILE* f) { if (f) fclose(f); });
    if (!pipe) {
        throw std::runtime_error("popen failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        command_result_.push_back(buffer.data());
    }
}

void NtpStatusCommandsWidget::update_content() {
    ntp_config_main_widget_->clear();
    auto header_first = ntp_config_main_widget_->addWidget(std::make_unique<Wt::WText>("Информация комманды timedatectl status"));
    header_first->setStyleClass("m-3 h4");

    auto list_group_first = ntp_config_main_widget_->addWidget(std::make_unique<Wt::WContainerWidget>());
    list_group_first->setStyleClass("list-group m-3");

    executeCommand("timedatectl status");
    for (auto result : command_result_)
    {
        auto row = list_group_first->addWidget(std::make_unique<WContainerWidget>());
        row->addStyleClass("list-group-item w-50");
        row->addWidget(std::make_unique<Wt::WText>(result));
    }

    auto header_second = ntp_config_main_widget_->addWidget(std::make_unique<Wt::WText>("Информация комманды timedatectl timesync-status"));
    header_second->setStyleClass("m-3 h4");

    auto list_group_second = ntp_config_main_widget_->addWidget(std::make_unique<Wt::WContainerWidget>());
    list_group_second->setStyleClass("list-group m-3");

    executeCommand("timedatectl timesync-status");
    for (auto result : command_result_)
    {
        auto row = list_group_second->addWidget(std::make_unique<WContainerWidget>());
        row->addStyleClass("list-group-item w-50");
        row->addWidget(std::make_unique<Wt::WText>(result));
    }
}

