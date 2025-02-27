#ifndef EDITTIMEWIDGET_H
#define EDITTIMEWIDGET_H
#include <Wt/WContainerWidget.h>
#include <Wt/WDateEdit.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WCalendar.h>
#include <Wt/WText.h>

class EditTimeWidget:public Wt::WContainerWidget {
public:
    EditTimeWidget();
    void set_edit_access(bool fl = true);
    std::string get_date();
    std::string get_time();
    void set_date_from_string(std::string date);
    void set_time_from_string(std::string time);
private:
    Wt::WDateEdit* date_edit_;
    Wt::WTimeEdit* time_edit_;
};

#endif //EDITTIMEWIDGET_H
