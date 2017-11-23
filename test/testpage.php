<?php

require_once 'class.Page.inc';
require_once 'class.FirmPage.inc';

$p = new Page(
        new Header(),
        new Navigation('firma'),
        FirmPage::choosePage(FirmPage::LIST_FIRM),
        new Footer());

$p->renderHTML();
