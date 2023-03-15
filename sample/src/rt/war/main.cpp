/*****************************************************************************
 * Copyright (C), 2023,  Computing & Graphics Research Institute of OPLUS Mobile Comm Corp., Ltd
 * License: No license is required for Oplus internal usage.
 *          No external usage is allowed.
 *
 * File : main.cpp
 *
 * Version: 2.0
 *
 * Date : Feb 2023
 *
 * Author: Computing & Graphics Research Institute
 *
 * ------------------ Revision History: ---------------------
 *  <version>  <date>  <author>  <desc>
 *
 *******************************************************************************/

#include "../../desktop/app.h"
#include "war.h"
#include <CLI11.hpp>

int main(int argc, const char * argv[]) {
    try {
        DesktopApp::Options dao;
        WarScene::Options   options;
        CLI::App            app("War");
        SETUP_COMMON_CLI11_OPTIONS(app, dao, options);
        app.add_option("model", options.model, "Specify path of the model.");
        PARSE_CLI11_OPTIONS(app, dao, argc, argv);
        run<WarScene>(dao, options);
        return 0;
    } catch (std::exception & ex) {
        PH_LOGE("%s\n", ex.what());
        return -1;
    } catch (...) {
        PH_LOGE("unknown exception...");
        return -1;
    }
}