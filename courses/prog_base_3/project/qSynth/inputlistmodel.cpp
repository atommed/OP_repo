#include "inputlistmodel.h"
#include <algorithm>
#include <QDebug>

namespace qSynth {

InputListModel::InputListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int InputListModel::rowCount(const QModelIndex&) const{
    return inputs.size();
}

QVariant InputListModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid() || role != Qt::DisplayRole) return QVariant();

    QString data = inputs[index.row()].getName();
    return data;
}

bool InputListModel::insertRow(int row, const QModelIndex &parent){
    beginInsertRows(parent, row,row+1);
    endInsertRows();
    return true;
}

bool InputListModel::addInput(InputListItem& item){
    QString name = item.getName();
    if(std::find_if(inputs.begin(),inputs.end(),[&name](InputListItem& o){
        return name == o.getName();
    }) != inputs.end()) return false;
    insertRow(inputs.size(),QModelIndex());
    inputs.push_back(item);
    return true;
}

void InputListModel::modifyItem(const QModelIndex &idx){
    if(!idx.isValid()) return;
    inputs.at(idx.row()).modify();
}

void InputListModel::removeItem(const QModelIndex &idx){
    if(!idx.isValid()) return;
    beginRemoveRows(idx,idx.row(),idx.row() + 1);
    inputs.erase(inputs.begin() + idx.row());
    endRemoveRows();
}

std::vector<GenericInputAction> InputListModel::getMultiplexedInput(){
    std::vector<GenericInputAction> multiplexed;
    std::vector<GenericInputAction> to_multiplex;
    for(InputListItem& item : inputs){
        QString name = item.getName();
        IGenericInput* input = item.getInput();
        if(!input->hasInput()) continue;
        to_multiplex = input->pollInput();
        for(GenericInputAction& a : to_multiplex){
            qDebug()<<name<<" plays "<<a.key;
            a.inputName = name;
        }
        multiplexed.insert(multiplexed.end(),to_multiplex.begin(),to_multiplex.end());
        to_multiplex.clear();
    }
    return multiplexed;
}

} // namespace qSynth

