#include <Wt/WApplication.h>
#include <Wt/WServer.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WBootstrap5Theme.h>

#include "MenuPage.h"

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment &env) {
    std::unique_ptr<Wt::WApplication> app = std::make_unique<Wt::WApplication>(env);

    app->setTitle("Конфигурирование NTP");

    app->setTheme(std::make_unique<Wt::WBootstrap5Theme>());

    app->root()->addWidget(std::make_unique<MenuPage>());

    return app;
}

int main(int argc, char **argv) {
    if (geteuid() != 0) {
        std::cerr << "Ошибка! Программа должна быть запущена с правами sudo!"<<std::endl;
        return 1;
    }
    return WRun(argc, argv, &createApplication);
}