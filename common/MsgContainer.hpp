#pragma once

#include <map>

namespace c2x {

    template <typename T, typename U>
    class MsgContainer {
        private:
            std::map<U, T*> *database_;

        public:
            MsgContainer() : database_(new std::map<U, T*>()) { }

            MsgContainer(const MsgContainer& other) {
                database_ = new std::map<U, T*>(*other.database_);
            }

            MsgContainer(MsgContainer&& other) noexcept {
                swap(*this, other);
            }

            ~MsgContainer() {
                for(auto it = database_->begin(); it != database_->end(); it++) {
                    freeElement((*it)->second());
                }

                delete database_;
            }

            MsgContainer& operator=(MsgContainer other) {
                swap(*this, other);
                return *this;
            }

            friend void swap(MsgContainer& db1, MsgContainer& db2) {
                using std::swap;
                swap(db1.database_, db2.database_);
            }

            virtual int add(T* element) {
                
            }

            virtual T* get(U index) {
                
            }

            virtual void freeElement(T* element) = 0;
    };
};