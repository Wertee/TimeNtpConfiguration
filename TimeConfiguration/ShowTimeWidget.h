#ifndef SHOWTIMEWIDGET_H
#define SHOWTIMEWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WDate.h>
#include <Wt/WTime.h>
#include <Wt/WTimer.h>
#include <Wt/WText.h>

class ShowTimeWidget: public Wt::WContainerWidget {
public:
    ShowTimeWidget();
private:
    Wt::WDate current_server_date_;
    Wt::WTime current_server_time_;
    Wt::WText* date_text_;
    Wt::WText* time_text_;
    std::unique_ptr<Wt::WTimer> timer_;
    void updateDateTime();
};

#endif //SHOWTIMEWIDGET_H
