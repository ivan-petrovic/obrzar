<?php

require_once 'util/class.Application.inc';
require_once 'page/class.Page.inc';
require_once 'page/class.Header.inc';
require_once 'page/class.Navigation.inc';
require_once 'page/class.Content.inc';
require_once 'page/class.Footer.inc';
require_once 'payment/class.PaymentPageDispatcher.inc';

session_start();

if(Application::issetFirm()) {
   $op = Application::getOp();
   $pageContent = PaymentPageDispatcher::choosePage($op, Application::getCurrFirmID(), Application::getPaymentID(), 3);
   $css = PaymentPageDispatcher::getCSSFileName($op);
   $pageContent->execute();
} else {
   $pageContent = new Content("Обрада обрачуна",
           "ИЗАБЕРИТЕ ФИРМУ за коју се врши обрада обрачуна.", 3);
}

$p = new Page(
        new Header(Application::getCurrFirmName(), Application::getCurrPaymentName(), $css),
        new Navigation(Navigation::MENU_PAYMENT),
        $pageContent,
        new Footer());

$p->renderHTML();
