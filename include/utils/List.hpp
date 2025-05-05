#pragma once
#include<list>
#include<memory>
#include<algorithm>
#include<vector>

template<typename _list,typename _list_node>
class list_node;
//custom list,客制化列表
template<typename _list,typename _list_node>
class clist;

template<typename _list,typename _list_node>
class list_node{
    friend class clist<_list,_list_node>;
    
    _list_node* prev;
    _list_node* next;
    _list* fat=nullptr;
    public:
    void SetParent(_list* _fat){fat=_fat;}
    virtual ~list_node(){
        if(fat!=nullptr)EraseFromParent();
    }
    list_node(){
        this->prev=nullptr;
        this->next=nullptr;
    }
    virtual void EraseFromParent(){
        if(this->prev==nullptr)fat->head=this->next;
        if(this->next==nullptr)fat->tail=this->prev;
        if(this->prev!=nullptr)this->prev->next=this->next;
        if(this->next!=nullptr)this->next->prev=this->prev;
        fat->size--;
        fat=nullptr;
        this->prev=nullptr;
        this->next=nullptr;
    }
    virtual _list* GetParent(){return this->fat;};

    virtual void replace_list_node_with(_list_node* other){
        if(this->prev==nullptr)fat->head=other;
        if(this->next==nullptr)fat->tail=other;
        if(this->prev!=nullptr)this->prev->next=other;
        if(this->next!=nullptr)this->next->prev=other;
        other->prev=this->prev;
        other->next=this->next;
        other->fat=this->fat;
        this->prev=nullptr;
        this->next=nullptr;
        this->fat=nullptr;
    }

    _list_node* nextListNode(){
        return next;
    }

    _list_node* prevListNode(){
        return prev;
    }
};

template<typename _list,typename _list_node>
class clist
{
    friend class list_node<_list,_list_node>;
    _list_node* head;
    _list_node* tail;
    int size=0;
    public:
    virtual ~mylist(){
        clear();
    }
    class iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = _list_node*;
        using difference_type = std::ptrdiff_t;
        using pointer = _list_node**;
        using reference = _list_node*&;
        _list_node* ptr;
        public:
        iterator():ptr(nullptr){}
        iterator(_list_node* _ptr):ptr(_ptr){}
        iterator& operator++(){
            ptr=ptr->next;
            return *this;
        }
        iterator& operator--(){
            ptr=ptr->prev;
            return *this;
        }
        value_type operator*(){return ptr;}
        iterator insert_before(_list_node* data){
            assert(ptr!=nullptr&&ptr->fat!=nullptr&&data!=nullptr&&"Invalid Iterator");
            if(ptr==ptr->fat->head)
                ptr->fat->push_front(data);
            else{
                data->SetParent(ptr->fat);
                data->prev=ptr->prev;
                data->next=ptr;
                ptr->prev=data;
                data->prev->next=data;
                ptr->fat->size++;
            }
            return iterator(data);
        }
        iterator insert_after(_list_node* data){
            assert(ptr!=nullptr&&ptr->fat!=nullptr&&data!=nullptr&&"Invalid Iterator");
            if(ptr==ptr->fat->tail)
                ptr->fat->push_back(data);
            else{
                data->SetParent(ptr->fat);
                data->next=ptr->next;
                data->prev=ptr;
                ptr->next=data;
                data->next->prev=data;
                ptr->fat->size++;
            }
            return iterator(data);
        }

        bool operator==(const iterator& other)const{return ptr==other.ptr;}
        bool operator!=(const iterator& other)const{return ptr!=other.ptr;}
    };
    clist(){
        this->head=nullptr;
        this->tail=nullptr;
    }
    virtual iterator begin(){return iterator(this->head);}
    virtual iterator end(){return iterator(nullptr);}
    virtual iterator rbegin(){return iterator(this->tail);}
    virtual iterator rend(){return iterator(nullptr);}
    void collect(_list_node* _begin,_list_node* _end){
        assert(this->head==nullptr&&this->tail==nullptr&&"Used to Separte List");
        this->head=_begin;
        this->tail=_end;
        size=0;
        for(auto poi=head;poi!=nullptr;poi=poi->next){
            poi->SetParent(dynamic_cast<_list*>(this));
            size++;
        }
    }

    std::pair<_list_node*,_list_node*> split(_list_node* _begin,_list_node* _end){
        assert(_begin!=nullptr&&_end!=nullptr&&"Invalid Split");
        assert(_begin->fat==dynamic_cast<_list*>(this)&&_end->fat==dynamic_cast<_list*>(this)&&"Invalid Split");
        if(_begin==this->head)this->head=_end->next;
        if(_end==this->tail)this->tail=_begin->prev;
        if(_begin->prev!=nullptr)_begin->prev->next=_end->next;
        if(_end->next!=nullptr)_end->next->prev=_begin->prev;
        _begin->prev=nullptr;
        _end->next=nullptr;

        size=0;
        for(auto poi=head;poi!=nullptr;poi=poi->next){
            size++;
        }

        return std::make_pair(_begin,_end);
    }
    int Size(){
        return size;
    }

    void push_back(_list_node* data){
        data->SetParent(dynamic_cast<_list*>(this));
        if(this->head==nullptr){
            this->head=data;
            this->tail=data;
        }
        else{
            this->tail->next=data;
            data->prev=this->tail;
            this->tail=data;
        }
        size++;
    }
    void push_front(_list_node* data){
        data->SetParent(dynamic_cast<_list*>(this));
        if(this->head==nullptr){
            this->head=data;
            this->tail=data;
        }
        else{
            this->head->prev=data;
            data->next=this->head;
            this->head=data;
        }
        size++;
    }

    void swap_region(_list_node* begin, _list_node* end, std::list<_list_node*>& sequence) {
        assert(!sequence.empty()&&"sequence can't be empty");
        
        auto prev=begin->prev;
        auto next=end;
        // auto next=end->next;
        
        if(prev==nullptr)
            head=sequence.front();

        // if(next!=nullptr)
        //     next=sequence.back();

        for(auto ele:sequence)
            assert(ele->GetParent()==this&&"All elements inside the sequence must be in this list");

        for(auto ele:sequence){
            ele->prev=prev;
            if(prev!=nullptr)
                prev->next=ele;
            prev=ele;
        }
        
        sequence.back()->next=next;
        if(next!=nullptr)
            next->prev=sequence.back();
    }

    virtual void clear(){
        while(this->head!=nullptr){
            delete head;
        }
    }
    _list_node* front(){return this->head;}
    _list_node* back(){return this->tail;}
};