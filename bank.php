<?php

require_once 'util/class.Application.inc';
require_once 'page/class.Page.inc';
require_once 'page/class.Header.inc';
require_once 'page/class.Navigation.inc';
require_once 'page/class.Footer.inc';
require_once 'bank/class.BankPageDispatcher.inc';

session_start();

$op = Application::getOp();
$pageContent = BankPageDispatcher::choosePage($op, Application::getBankID(), 3);
$css = BankPageDispatcher::getCSSFileName($op);
$pageContent->execute();

$p = new Page(
        new Header(Application::getCurrFirmName(), Application::getCurrPaymentName(),$css),
        new Navigation(Navigation::MENU_BANK),
        $pageContent,
        new Footer());

$p->renderHTML();
