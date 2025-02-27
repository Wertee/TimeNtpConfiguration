#include "MenuPage.h"

#include "StatusCommands/NtpStatusCommandsWidget.h"
#include "TimeConfiguration/TimeWidget.h"

MenuPage::MenuPage() {
    auto tabWidget = std::make_unique<Wt::WTabWidget>();

    tabWidget->addTab(std::make_unique<TimeWidget>(), "Настройки времени");

    auto ntpConfig = std::make_unique<NtpStatusCommandsWidget>();
    auto ntpConfigPtr = ntpConfig.get();
    tabWidget->addTab(std::move(ntpConfig), "Статус - Точное время");

    tabWidget->currentChanged().connect([tabWidget = tabWidget.get(), ntpConfigPtr](int index) {
        if (tabWidget->indexOf(ntpConfigPtr) == index) {
            ntpConfigPtr->update_content();
        }
    });

    this->addWidget(std::move(tabWidget));
}
