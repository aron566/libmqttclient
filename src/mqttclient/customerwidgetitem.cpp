/**
  ******************************************************************************
  * @file    customerwidgetitem.cpp
  * @author  aron566
  * @version v1.0
  * @date    2020-08-24
  * @brief   label-pushbutton标签按钮组.
  ******************************************************************************
  */
/* Header includes -----------------------------------------------------------*/
#include "customerwidgetitem.h"
#include <QHBoxLayout>
/* Macro definitions ---------------------------------------------------------*/
/* Type definitions ----------------------------------------------------------*/
/* Variable declarations -----------------------------------------------------*/
/* Variable definitions ------------------------------------------------------*/
/* Function declarations -----------------------------------------------------*/
/* Function definitions ------------------------------------------------------*/
/**
 * @brief customerwidgetitem::customerwidgetitem
 * @param parent
 */
customerwidgetitem::customerwidgetitem(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    btn_del = new QPushButton;
    text = new QLineEdit;
    text->setEnabled(false);
    layout->addWidget(text);
    layout->addWidget(btn_del);
    setLayout(layout);
}
/* ---------------------------- end of file ----------------------------------*/
