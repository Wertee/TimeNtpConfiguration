#include "EditTimeWidget.h"

EditTimeWidget::EditTimeWidget()
{
    this->addStyleClass("row justify-content-start");

    auto col1 = this->addWidget(std::make_unique<Wt::WContainerWidget>());
    col1->addStyleClass("col-auto m-2");
    col1->addWidget(std::make_unique<Wt::WText>("Введите дату и время"));

    auto col2 = this->addWidget(std::make_unique<Wt::WContainerWidget>());
    col2->addStyleClass("col-auto m-2");
    date_edit_ = col2->addWidget(std::make_unique<Wt::WDateEdit>());
    date_edit_->setToolTip("Укажите дату для установки");
    auto date_validator = std::make_shared<Wt::WDateValidator>();
    date_edit_->setValidator(date_validator);

    date_edit_->changed().connect([this, date_validator] {
       auto result_validation = date_validator->validate(date_edit_->text());
        if (result_validation.state() != Wt::ValidationState::Valid) {
            date_edit_->setDate(Wt::WDate::currentServerDate());
        }
    });

    auto col3 = this->addWidget(std::make_unique<Wt::WContainerWidget>());
    col3->addStyleClass("col-auto m-2");
    time_edit_ = col3->addWidget(std::make_unique<Wt::WTimeEdit>());
    time_edit_->setToolTip("Укажите время для установки");
    auto time_validator = std::make_shared<Wt::WTimeValidator>();
    time_edit_->setValidator(time_validator);
    time_edit_->changed().connect([this, time_validator] {
        auto result_validation = time_validator->validate(time_edit_->text());
        if (result_validation.state() != Wt::ValidationState::Valid) {
            time_edit_->setTime(Wt::WTime::currentServerTime());
        }
    });
}

void EditTimeWidget::set_edit_access(bool fl)
{
    if (fl)
    {
        date_edit_->setDisabled(false);
        time_edit_->setDisabled(false);
    }
    else
    {
        date_edit_->setDisabled(true);
        time_edit_->setDisabled(true);
    }
}

std::string EditTimeWidget::get_date()
{
    return date_edit_->text().toUTF8();
}

std::string EditTimeWidget::get_time()
{
    return time_edit_->text().toUTF8();
}

void EditTimeWidget::set_date_from_string(std::string date) {
    date_edit_->setDate(Wt::WDate::fromString(date,"yyyy-MM-dd"));
}

void EditTimeWidget::set_time_from_string(std::string time) {
    time_edit_->setTime(Wt::WTime::fromString(time));
}



