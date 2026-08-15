# Minimal recovery-profile distribution surface. This does not freeze the C ABI
# or claim a general package-management/distribution contract.
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(BUILD_RECOVERY_NATIVE_SDK)
    install(TARGETS synq_lib
        EXPORT SynQTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/synq"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    configure_package_config_file(
        "${CMAKE_CURRENT_LIST_DIR}/SynQConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/SynQConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/SynQ"
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/SynQConfigVersion.cmake"
        VERSION "0.1.0"
        COMPATIBILITY AnyNewerVersion
    )

    install(EXPORT SynQTargets
        FILE SynQTargets.cmake
        NAMESPACE SynQ::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/SynQ"
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/SynQConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/SynQConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/SynQ"
    )
endif()

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
    "${CMAKE_CURRENT_SOURCE_DIR}/../docs/EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md"
    "${CMAKE_CURRENT_SOURCE_DIR}/../docs/EXPERIMENTAL_NATIVE_SDK.md"
    DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/synq)

set(CPACK_PACKAGE_NAME "synq-recovery-cli")
set(CPACK_PACKAGE_VENDOR "SynQ Contributors")
set(CPACK_PACKAGE_VERSION "0.1.0-experimental")
set(CPACK_GENERATOR "ZIP")
include(CPack)
