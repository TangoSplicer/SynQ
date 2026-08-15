# Minimal recovery-profile distribution surface. This does not freeze the C ABI
# or claim a general package-management/distribution contract.
include(GNUInstallDirs)

if(TARGET synqc)
    install(TARGETS synqc RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/plugins")
    install(DIRECTORY plugins/ DESTINATION ${CMAKE_INSTALL_DATADIR}/synq/plugins FILES_MATCHING PATTERN "*.json")
endif()

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/../docs/CLI.md"
    "${CMAKE_CURRENT_SOURCE_DIR}/../docs/BOUNDED_CONSTANT_EVALUATION.md"
    "${CMAKE_CURRENT_SOURCE_DIR}/../docs/BOUNDED_SIMULATION.md"
    DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/synq)

set(CPACK_PACKAGE_NAME "synq-recovery-cli")
set(CPACK_PACKAGE_VENDOR "SynQ Contributors")
set(CPACK_PACKAGE_VERSION "0.1.0-experimental")
set(CPACK_GENERATOR "ZIP")
include(CPack)
