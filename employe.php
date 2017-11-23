<?php

require_once 'util/class.Application.inc';
require_once 'page/class.Page.inc';
require_once 'page/class.Header.inc';
require_once 'page/class.Navigation.inc';
require_once 'page/class.Content.inc';
require_once 'page/class.Footer.inc';
require_once 'employe/class.EmployePageDispatcher.inc';

session_start();

if(Application::issetFirm()) {
   $op = Application::getOp();
   $pageContent = EmployePageDispatcher::choosePage($op, Application::getCurrFirmID(), Application::getEmployeID(), 3);
   $css = EmployePageDispatcher::getCSSFileName($op);
   $pageContent->execute();
} else {
   $pageContent = new Content("Обрада запослених",
           "ИЗАБЕРИТЕ ФИРМУ за коју се врши обрада запослених.", 3);
   $css = "";
}

$p = new Page(
        new Header(Application::getCurrFirmName(), Application::getCurrPaymentName(), $css),
        new Navigation(Navigation::MENU_EMPLOYE),
        $pageContent,
        new Footer());

$p->renderHTML();
