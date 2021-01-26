#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <QObject>

class Producto : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int codigo READ codigo)
    Q_PROPERTY(float precio READ precio)
    Q_PROPERTY(QString nombre READ nombre)
public:
    explicit Producto(QObject *parent = nullptr);
    Producto(int codigo, QString nombre, float precio);
    // Getters
    int codigo() const;
    float precio() const;
    QString nombre() const;
    int decimals() const;

signals:
private:
    int m_codigo;
    float m_precio;
    QString m_nombre;
    int m_decimals;

};

#endif // PRODUCTO_H
