# Panko - GUI Resources - `txt`

This directory tree contains chunks of text that need to be updated, generated or localized but need to be included in the resource system.

These are included in the [resource file] like so:

```
<qresource prefix="/txt">
	<!-- Existing "Default" Text (en_US) -->
	<file alias="resource-name">./txt/en_US/resource-name.txt</file>
</qresource>
<qresource lang="en" prefix="/txt">
	<!-- Existing english Text -->
	<file alias="resource-name">./txt/en_US/resource-name.txt</file>
</qresource>
<qresource lang="fr" prefix="/txt">
	<!-- Existing French Text -->
	<file alias="resource-name">./txt/fr_FR/resource-name.txt</file>
</qresource>
```

If a file should not be localized, for instance [contributor names], then it lives here in the root.

[resource file]: ../panko.qrc
[contributor names]: ./contributors.txt
