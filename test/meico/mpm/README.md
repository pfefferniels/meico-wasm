# MPM (Musical Performance Markup) Test Suite

This directory contains a comprehensive test suite for the MPM functionality in the meico-wasm project. The tests validate that MPM documents can correctly modify MSM (Musical Sequence Markup) documents to create expressive musical performances.

## Test Structure

### Core Test Files

- **`MpmTestUtils.java`** - Utility class providing helper methods for creating test MSM and MPM documents
- **`MpmTestRunner.java`** - Main test runner that executes all MPM tests

### Map Type Tests

- **`ArticulationMapTest.java`** - Tests articulation maps (staccato, accent, etc.)
- **`DynamicsMapTest.java`** - Tests dynamics maps (forte, piano, crescendo, etc.)
- **`TempoMapTest.java`** - Tests tempo maps (allegro, andante, accelerando, etc.)
- **`MetricalAccentuationMapTest.java`** - Tests metrical accentuation patterns
- **`OrnamentationMapTest.java`** - Tests ornamentation (trills, grace notes, etc.)

### Integration Tests

- **`MpmIntegrationTest.java`** - Tests combining multiple map types and multi-part scenarios

## Test Coverage

### Map Types Covered
✅ **ArticulationMap** - Basic and note-specific articulation
✅ **DynamicsMap** - Basic dynamics and crescendo patterns  
✅ **TempoMap** - Basic tempo and accelerando patterns
✅ **MetricalAccentuationMap** - Rhythmic accentuation patterns
✅ **OrnamentationMap** - Basic ornamentation and trills
✅ **Integration** - Multiple maps working together

### Test Scenarios

1. **Basic Functionality Tests** - Single map type applied to simple MSM
2. **Pattern Tests** - Complex patterns like crescendo and accelerando  
3. **Multi-Part Tests** - Different parts with different performance instructions
4. **Integration Tests** - Multiple map types applied simultaneously

## Running the Tests

### Run All Tests
```bash
cd /path/to/meico-wasm
javac -cp "out/production/meico:externals/*" -d out/production/meico test/meico/mpm/*.java
java -cp "out/production/meico:externals/*" meico.mpm.MpmTestRunner
```

### Run Individual Test Classes
```bash
java -cp "out/production/meico:externals/*" meico.mpm.ArticulationMapTest
java -cp "out/production/meico:externals/*" meico.mpm.DynamicsMapTest
java -cp "out/production/meico:externals/*" meico.mpm.TempoMapTest
# etc.
```

## Test Results Summary

All tests demonstrate that MPM processing correctly:

1. **Applies Performance Attributes** - Adds `date.perf`, `duration.perf`, `milliseconds.date`, etc.
2. **Modifies Musical Parameters** - Changes velocity, timing, and expression
3. **Handles Multiple Parts** - Processes different instruments independently  
4. **Integrates Multiple Maps** - Combines effects from multiple performance instructions
5. **Creates Realistic Output** - Produces musically plausible performance variations

## Key Test Observations

### ArticulationMap
- Successfully processes articulation instructions
- Adds performance timing attributes
- Maintains musical structure while adding expression

### DynamicsMap  
- Correctly modifies note velocities
- Creates proper crescendo/diminuendo effects
- Generates channel volume maps for MIDI compatibility

### TempoMap
- Produces accurate millisecond timing based on BPM
- Creates proper accelerando/ritardando effects
- Shows correct tempo ratio changes over time

### Integration
- Multiple maps work together without conflicts
- Each map contributes its specific performance aspect
- Final output shows combined effects of all applied maps

## Validation Approach

The tests validate MPM functionality by:

1. **Creating Simple Test Data** - Basic MSM with known note patterns
2. **Applying MPM Instructions** - Using various map types with realistic parameters
3. **Analyzing Output** - Checking for expected performance attributes and value changes
4. **Verifying Plausibility** - Ensuring musical sense in the transformed output

## Future Extensions

Additional test areas that could be added:

- **RubatoMap** - Expressive timing flexibility
- **AsynchronyMap** - Part synchronization effects  
- **ImprecisionMap** - Humanization effects
- **MovementMap** - Spatial audio positioning
- **Style Tests** - Predefined performance styles
- **Complex Integration** - Large-scale musical works
- **Error Handling** - Invalid input scenarios

## Technical Notes

- Tests use XOM library for XML processing
- Performance objects render MSM modifications
- All tests create temporary data structures (no file I/O required)
- Test validation focuses on attribute presence and value reasonableness
- Output analysis uses XPath queries for efficiency

This test suite provides comprehensive validation of MPM functionality and serves as both quality assurance and documentation of expected behavior.