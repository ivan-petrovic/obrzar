<?php

require_once 'util/class.Application.inc';
require_once 'page/class.Page.inc';
require_once 'page/class.Header.inc';
require_once 'page/class.Navigation.inc';
require_once 'page/class.Footer.inc';
require_once 'shop/class.ShopPageDispatcher.inc';

session_start();

$op = Application::getOp();
$pageContent = ShopPageDispatcher::choosePage($op, Application::getShopID(), 3);
$css = ShopPageDispatcher::getCSSFileName($op);
$pageContent->execute();

$p = new Page(
        new Header(Application::getCurrFirmName(), Application::getCurrPaymentName(), $css),
        new Navigation(Navigation::MENU_SHOP),
        $pageContent,
        new Footer());

$p->renderHTML();
