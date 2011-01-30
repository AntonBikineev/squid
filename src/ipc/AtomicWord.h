/*
 * $Id$
 *
 */

#ifndef SQUID_IPC_ATOMIC_WORD_H
#define SQUID_IPC_ATOMIC_WORD_H

template <class Value>
class AtomicWordT {
public:
    AtomicWordT() {} // leave value unchanged
    AtomicWordT(Value aValue): value(aValue) {} // XXX: unsafe

    Value operator +=(int delta) { return __sync_add_and_fetch(&value, delta); }
    Value operator ++() { return *this += 1; }
    Value operator --() { return *this += -1; }
    Value operator ++(int) { return __sync_fetch_and_add(&value, 1); }
    Value operator --(int) { return __sync_fetch_and_add(&value, -1); }

    bool swap_if(const int comparand, const int replacement) { return __sync_bool_compare_and_swap(&value, comparand, replacement); }

    // TODO: no need for __sync_bool_compare_and_swap here?
    bool operator ==(int v2) { return __sync_bool_compare_and_swap(&value, v2, value); }

    // TODO: no need for __sync_fetch_and_add here?
    operator Value () const { return __sync_fetch_and_add(const_cast<Value*>(&value), 0); }

private:
    Value value;
};

typedef AtomicWordT<int> AtomicWord;

#endif // SQUID_IPC_ATOMIC_WORD_H
