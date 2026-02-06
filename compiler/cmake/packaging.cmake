# Only install synq target if it exists
if(TARGET synq)
    install(TARGETS synq DESTINATION bin)
endif()

install(DIRECTORY plugins/ DESTINATION share/synq/plugins FILES_MATCHING PATTERN "*.json")

set(CPACK_GENERATOR "ZIP")
include(CPack)
