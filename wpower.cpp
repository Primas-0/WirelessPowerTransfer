// UMBC - CMSC 341 - Spring 2024 - Proj2
#include "wpower.h"
WirelessPower::WirelessPower(TREETYPE type){

}
WirelessPower::~WirelessPower(){

}
void WirelessPower::clear(){

}
void WirelessPower::insert(const Customer& customer){

}
void WirelessPower::remove(int id){

}
TREETYPE WirelessPower::getType() const{

}
void WirelessPower::setType(TREETYPE type){

}
const WirelessPower & WirelessPower::operator=(const WirelessPower & rhs){

}
void WirelessPower::dumpTree() const {dump(m_root);}
void WirelessPower::dump(Customer* customer) const{
    if (customer != nullptr){
        cout << "(";
        dump(customer->m_left); //first visit the left child
        cout << customer->m_id << ":" << customer->m_height;//second visit the node itself
        dump(customer->m_right);//third visit the right child
        cout << ")";
    }
}
ostream& operator<<(ostream& sout, const Customer & x ) {
    sout << x.m_id << " (" << x.getLatStr() << ", " << x.getLongStr() << ")" << endl;;
    return sout;
}