#include "tienda.h"
#include "ui_tienda.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QString>
#include<QDoubleSpinBox>
#include "factura.h"
Tienda::Tienda(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Tienda)
{
    ui->setupUi(this);
    m_subtotal = 0;
    cargarDatos();
    inicializarWidgets();
}

Tienda::~Tienda()
{
    delete ui;
}

void Tienda::mostrarPrecio(int indice)
{
    // Obtener el producto que se a seleccionado en el combobox
    float precioProducto = m_productos.at(indice)->precio();

    // Mostrar el precio en el widget correspondiente
    ui->outPrecio->setText("$" + QString::number(precioProducto));
}

void Tienda::cargarDatos()
{
    // Crear y agregar productos a la lista
    //m_productos.append(new Producto(1, "Pan", 0.15));
    //m_productos.append(new Producto(2, "Leche", 0.80));
    //m_productos.append(new Producto(3, "Huevos", 0.12));

    QString pathActual = QDir::currentPath();

    QFile archivo(pathActual + "/productos.csv");

    bool primeralinea = true;
    if(archivo.open(QFile::ReadOnly)){
        QTextStream in(&archivo);
        while (!in.atEnd()) {
            QString linea = in.readLine();
            if (primeralinea){
                primeralinea = false;
                continue;
            }
            // separar los datos con ";"
            QStringList datos = linea.split(";");
            int codigo = datos[0].toInt();
            float precio = datos[2].toFloat();
            //Crear producto
            m_productos.append(new Producto(codigo,datos[1],precio));
        }

    }else{
        QMessageBox msgBox;
        msgBox.setText("El archivo de productos no se puede abrir");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }


}


void Tienda::inicializarWidgets()
{
    // Agregar los productos al combo box
    for(int i=0; i<m_productos.length(); i++){
        ui->inProducto->addItem(m_productos.at(i)->nombre());
    }
    //Colocar cabecera a la tabla
    QStringList cabecera = {"Cantidad", "Productos", "Subtotal"};
    ui->outDetalle->setColumnCount(3);
    ui->outDetalle->setHorizontalHeaderLabels(cabecera);

    //Eventos
    connect(ui->inProducto, SIGNAL(currentIndexChanged(int)),
                this, SLOT(mostrarPrecio(int)));
    // mostrar el precio del primer producto
    mostrarPrecio(0);
}

void Tienda::calcular(float subtotal)
{
    // Calcular valores
    m_subtotal += subtotal;
    float iva = m_subtotal * IVA;
    float total = m_subtotal + iva;
    // Mostrar valores en UI
    ui->outSubTotal->setText(QString::number(m_subtotal));
    ui->outIva->setText(QString:: number(iva));
    ui->outTotal->setText(QString::number(total));

}

void Tienda::validarCedula()
{
    QString cedula = ui->inCedula->text();
    if(cedula.length()!=10){
    ui->inCedula->setStyleSheet("background-color: rgb(239, 41, 41);");
    }else{
    ui->inCedula->setStyleSheet("background-color: rgb(132, 225, 113);");
    }
}

void Tienda::validarNombre()
{
    QString nombre = ui->inCedula->text();
    if(nombre.length()!=10){
    ui->inNombre->setStyleSheet("background-color: rgb(219, 41, 41);");
    }else{
    ui->inNombre->setStyleSheet("background-color: rgb(222, 225, 113);");
    }
}

void Tienda::on_cmdAgregar_clicked()
{
    // Obtener el producto seleccionado
      int indice = ui->inProducto->currentIndex();
      Producto *p = m_productos.at(indice);
      // Obtener la cantidad
      int cantidad = ui->inCantidad->value();

      // Validar: si la cantidad es 0, no hace nada
      if (cantidad == 0){
          return;
}
      // Calcular el subtotal
          float subtotal = cantidad * p->precio();

          // Agregar a la tabla
          int posicion = ui->outDetalle->rowCount();
          ui->outDetalle->insertRow(posicion);
          ui->outDetalle->setItem(posicion,0,new QTableWidgetItem(QString::number(cantidad)));
          ui->outDetalle->setItem(posicion,1,new QTableWidgetItem(p->nombre()));
          ui->outDetalle->setItem(posicion,2,new QTableWidgetItem(QString::number(subtotal)));

          // Limpiar widgets
          ui->inCantidad->setValue(0);
          ui->inProducto->setFocus();

          // calcular
          calcular(subtotal);

}

void Tienda::on_inCedula_editingFinished()
{
    validarCedula();
}

void Tienda::on_inNombre_editingFinished()
{
    validarNombre();
}


void Tienda::on_cmdFinalizar_clicked()
{
    Factura *dialogFactura=new Factura();
    dialogFactura->show();
}
