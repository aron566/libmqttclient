/**
 *  @file customerwidgetitem.h
 *
 *  @date 2020-08-27
 *
 *  @author aron566
 *
 *  @brief None
 *
 *  @version V1.0
 */
#ifndef CUSTOMERWIDGETITEM_H
#define CUSTOMERWIDGETITEM_H
/** Includes -----------------------------------------------------------------*/
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
/** Private includes ---------------------------------------------------------*/

/** Private defines ----------------------------------------------------------*/

/** Exported typedefines -----------------------------------------------------*/

/** Exported constants -------------------------------------------------------*/

/** Exported macros-----------------------------------------------------------*/
/** Exported variables -------------------------------------------------------*/
/** Exported functions prototypes --------------------------------------------*/
/**
 * @brief The customerwidgetitem class
 */
class customerwidgetitem : public QWidget
{
    Q_OBJECT
public:
    explicit customerwidgetitem(QWidget *parent = nullptr);

signals:

public slots:
public:
    QLineEdit   *text;
    QPushButton *btn_del;
};

#endif // CUSTOMERWIDGETITEM_H

/******************************** End of file *********************************/
