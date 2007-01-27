/*MT*
    
    MediaTomb - http://www.mediatomb.org/
    
    tasks.cc - this file is part of MediaTomb.
    
    Copyright (C) 2005 Gena Batyan <bgeradz@mediatomb.org>,
                       Sergey 'Jin' Bostandzhyan <jin@mediatomb.org>
    
    Copyright (C) 2006-2007 Gena Batyan <bgeradz@mediatomb.org>,
                            Sergey 'Jin' Bostandzhyan <jin@mediatomb.org>,
                            Leonhard Wimmer <leo@mediatomb.org>
    
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.
    
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    version 2 along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    
    $Id$
*/

/// \file tasks.cc

#ifdef HAVE_CONFIG_H
    #include "autoconfig.h"
#endif

#include "pages.h"
#include "common.h"
#include "content_manager.h"

using namespace zmm;
using namespace mxml;

void web::tasks::process()
{
    check_request();
    String action = param(_("action"));
    if (! string_ok(action))
        throw _Exception(_("web:tasks called with illegal action"));
    Ref<ContentManager> cm = ContentManager::getInstance();
    
    if (action == "current")
    {
        Ref<Element> tasksEl (new Element(_("tasks")));
        appendTask(tasksEl, cm->getCurrentTask());
        root->appendChild(tasksEl);
    }
    else if (action == "list")
    {
        Ref<Element> tasksEl (new Element(_("tasks")));
        root->appendChild(tasksEl);
        Ref<Array<CMTask> > taskList = cm->getTasklist();
        if (taskList == nil)
            return;
        int count = taskList->size();
        for (int i = 0; i < count; i++)
        {
            appendTask(tasksEl, taskList->get(i));
        }
    }
    else if (action == "cancel")
    {
        int taskID = intParam(_("taskID"));
        cm->invalidateTask(taskID);
    }
    else
        throw _Exception(_("web:tasks called with illegal action"));
}

void web::tasks::appendTask(Ref<Element> el, Ref<CMTask> task)
{
    if (task == nil || el == nil)
        return;
    Ref<Element> taskEl (new Element(_("task")));
    taskEl->addAttribute(_("id"), String::from(task->getID())); 
    taskEl->setText(task->getDescription());
    el->appendChild(taskEl);
}