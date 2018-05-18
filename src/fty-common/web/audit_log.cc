/*  =========================================================================
    fty_common_web_audit_log - Manage audit log

    Copyright (C) 2014 - 2018 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    =========================================================================
*/

/*
@header
    fty_common_web_audit_log - Manage audit log
@discuss
@end
*/

#include "fty_common_classes.h"

Ftylog AuditLogManager::_auditlog = Ftylog("www-audit", Ftylog::DEFAULT_LOG_CONFIG_FILE);

//  return audit logger
Ftylog* AuditLogManager::getInstance()
{
  return &_auditlog;
}

void AuditLogManager::setAuditLogContext(const std::string token, const std::string username, const int userId)
{
  // Prepare context params for audit-log
  std::hash<std::string> hash_token;
  size_t contextToken = hash_token(token);
  std::map<std::string, std::string> contextParam;
  contextParam.insert(std::make_pair("sessionid", std::to_string(contextToken).substr(11)));
  contextParam.insert(std::make_pair("username", username));
  contextParam.insert(std::make_pair("uid", std::to_string(userId)));
  
  // Set the fty-log context
  Ftylog::setContext(contextParam);
}

void AuditLogManager::clearAuditLogContext()
{
  Ftylog::clearContext();
}