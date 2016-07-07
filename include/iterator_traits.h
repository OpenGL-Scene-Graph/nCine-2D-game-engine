#ifndef NCINE_ITERATOR_TRAITS
#define NCINE_ITERATOR_TRAITS

namespace ncine {

/// Dispatching tag for iterators that can only move forward, one element at a time
struct ForwardIteratorTag { };
/// Dispatching tag for iterators that can move both ways, one element at a time
struct BidirectionalIteratorTag : public ForwardIteratorTag { };
/// Dispatching tag for iterators that can jump arbitrary distances in both ways
struct RandomAccessIteratorTag : public BidirectionalIteratorTag { };

/// Base iterator traits structure
template <class Iterator> struct IteratorTraits { };

}

#endif