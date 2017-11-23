<?php

require_once 'util/class.Application.inc';
require_once 'page/class.Page.inc';
require_once 'page/class.Header.inc';
require_once 'page/class.Navigation.inc';
require_once 'page/class.Content.inc';
require_once 'page/class.Footer.inc';
require_once 'report/class.ReportPageDispatcher.inc';

session_start();

$css = "";
if(Application::issetFirm()) {
   if(Application::issetPayment()) {
      $op = Application::getOp();
      $pageContent = ReportPageDispatcher::choosePage($op, Application::getCurrFirmID(),
              Application::getCurrPaymentID(), Application::getEmployeID(), 3);
      $css = ReportPageDispatcher::getCSSFileName($op);
      // $pageContent->execute();
   } else {
      $pageContent = new Content("Извештаји",
              "ИЗАБЕРИТЕ ОБРАЧУН за који желите да прегледате извештаје.", 3);
   }
} else {
   $pageContent = new Content("Извештаји",
           "ИЗАБЕРИТЕ ФИРМУ за коју желите да прегледате извештаје.", 3);
}

$p = new Page(
        new Header(Application::getCurrFirmName(), Application::getCurrPaymentName(), $css),
        new Navigation(Navigation::MENU_REPORT),
        $pageContent,
        new Footer());

$p->renderHTML();
