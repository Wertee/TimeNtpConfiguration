#include "ShowTimeWidget.h"

ShowTimeWidget::ShowTimeWidget()
{
    this->addStyleClass("row justify-content-start");

    auto col1 = this->addWidget(std::make_unique<Wt::WContainerWidget>());
    col1->addStyleClass("col-auto m-2");
    col1->addWidget(std::make_unique<Wt::WText>("Текущая дата и время"));

    auto col2 = this->addWidget(std::make_unique<Wt::WContainerWidget>());
    col2->addStyleClass("col-auto m-2");
    date_text_ = col2->addWidget(std::make_unique<Wt::WText>());
    date_text_->addStyleClass("h3");

    auto col3 = this->addWidget(std::make_unique<Wt::WContainerWidget>());
    col3->addStyleClass("col-auto m-2");
    time_text_ = col3->addWidget(std::make_unique<Wt::WText>());
    time_text_->setStyleClass("h3");

    timer_ = std::make_unique<Wt::WTimer>();
    timer_->setInterval(std::chrono::seconds(1));
    timer_->timeout().connect([this] {updateDateTime();});

    updateDateTime();
    timer_->start();
}

void ShowTimeWidget::updateDateTime()
{
    current_server_date_ = Wt::WDate::currentServerDate();
    current_server_time_ = Wt::WTime::currentServerTime();

    date_text_->setText(current_server_date_.toString("yyyy-MM-dd"));
    time_text_->setText(current_server_time_.toString("HH:mm:ss"));
}
