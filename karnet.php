<?php

require_once 'util/class.Application.inc';
require_once 'page/class.Page.inc';
require_once 'page/class.Header.inc';
require_once 'page/class.Navigation.inc';
require_once 'page/class.Content.inc';
require_once 'page/class.Footer.inc';
require_once 'karnet/class.KarnetPageDispatcher.inc';

session_start();

$css = "";
if(Application::issetFirm()) {
   if(Application::issetPayment()) {
      $op = Application::getOp();
      $pageContent = KarnetPageDispatcher::choosePage($op, Application::getCurrFirmID(),
              Application::getCurrPaymentID(), Application::getEmployeID(), 3);
      $css = KarnetPageDispatcher::getCSSFileName($op);
      $pageContent->execute();
   } else {
      $pageContent = new Content("Обрада карнета",
              "ИЗАБЕРИТЕ ОБРАЧУН за који се врши обрада карнета.", 3);
   }
} else {
   $pageContent = new Content("Обрада карнета",
           "ИЗАБЕРИТЕ ФИРМУ за коју се врши обрада карнета.", 3);
}

$p = new Page(
        new Header(Application::getCurrFirmName(), Application::getCurrPaymentName(), $css),
        new Navigation(Navigation::MENU_KARNET),
        $pageContent,
        new Footer());

$p->renderHTML();
