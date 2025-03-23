## TODO:
- Implement FixedPointArithmetic <- By myself lmao
- Make sure bit shifting operations work with all endianess
- 

## Design Notes

- The reason why `uint64` is used for EntityID is to avoid narrowing. We use
  `std::vector::size` for getting unique EntityIDs, which outputs `uint64`.
