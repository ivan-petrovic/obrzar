<?php

require_once 'util/class.Application.inc';
require_once 'page/class.Page.inc';
require_once 'page/class.Header.inc';
require_once 'page/class.Navigation.inc';
require_once 'page/class.Footer.inc';
require_once 'firm/class.FirmPageDispatcher.inc';

session_start();

$op = Application::getOp();
$pageContent = FirmPageDispatcher::choosePage($op, Application::getFirmID(), 3);
$css = FirmPageDispatcher::getCSSFileName($op);
$pageContent->execute();

$p = new Page(
        new Header(Application::getCurrFirmName(), Application::getCurrPaymentName(), $css),
        new Navigation(Navigation::MENU_FIRM),
        $pageContent,
        new Footer());

$p->renderHTML();
