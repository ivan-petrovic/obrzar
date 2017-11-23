<?php

require_once 'util/class.Application.inc';
require_once 'page/class.Page.inc';
require_once 'page/class.Header.inc';
require_once 'page/class.Navigation.inc';
require_once 'page/class.Content.inc';
require_once 'page/class.Footer.inc';
require_once 'deduction/class.DeductionPageDispatcher.inc';

session_start();

if(Application::issetFirm()) {
   $op = Application::getOp();
   $pageContent = DeductionPageDispatcher::choosePage($op, Application::getDeductionID(), 3);
   $css = DeductionPageDispatcher::getCSSFileName($op);
   $pageContent->execute();
} else {
   $pageContent = new Content("Обрада обустава",
           "ИЗАБЕРИТЕ ФИРМУ за коју се врши обрада запослених и обустава.", 3);
}

$p = new Page(
        new Header(Application::getCurrFirmName(), Application::getCurrPaymentName(), $css),
        new Navigation(Navigation::MENU_DEDUCTION),
        $pageContent,
        new Footer());

$p->renderHTML();
