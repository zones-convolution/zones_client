import { type ReactNode, useCallback } from "react";

import {
  Pagination,
  PaginationContent,
  PaginationEllipsis,
  PaginationItem,
  PaginationLink,
  PaginationNext,
  PaginationPrevious,
} from "@/components/ui/pagination";
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from "@/components/ui/select";

import { useSearchContext } from "@/context/search_context";
import { cn } from "@/lib/utils";

export interface PaginationWithLinksProps {
  pageSizeOptions: number[];
}

export const ZonesPaginationWithLinks = ({
  pageSizeOptions,
}: PaginationWithLinksProps) => {
  const {
    search: { count },
    params: { page, pageSize },
    setParams,
  } = useSearchContext();

  const totalPageCount = Math.ceil(count / pageSize);

  // Update the page number in the query params
  const buildLink = useCallback(
    (newPage: number) => {
      setParams((p) => ({ ...p, page: newPage }));
    },
    [setParams],
  );

  // Update the page size in the query params
  const navToPageSize = useCallback(
    (newPageSize: number) => {
      setParams((p) => ({ ...p, pageSize: newPageSize }));
    },
    [setParams],
  );

  const renderPageNumbers = () => {
    const items: ReactNode[] = [];
    const maxVisiblePages = 5;

    if (totalPageCount <= maxVisiblePages) {
      for (let i = 1; i <= totalPageCount; i++) {
        items.push(
          <PaginationItem key={i}>
            <PaginationLink
              href="#"
              isActive={page === i}
              onClick={() => buildLink(i)}
            >
              {i}
            </PaginationLink>
          </PaginationItem>,
        );
      }
    } else {
      items.push(
        <PaginationItem key={1}>
          <PaginationLink
            href="#"
            isActive={page === 1}
            onClick={() => buildLink(1)}
          >
            1
          </PaginationLink>
        </PaginationItem>,
      );

      if (page > 3) {
        items.push(
          <PaginationItem key="ellipsis-start">
            <PaginationEllipsis />
          </PaginationItem>,
        );
      }

      const start = Math.max(2, page - 1);
      const end = Math.min(totalPageCount - 1, page + 1);

      for (let i = start; i <= end; i++) {
        items.push(
          <PaginationItem key={i}>
            <PaginationLink
              href="#"
              isActive={page === i}
              onClick={() => buildLink(i)}
            >
              {i}
            </PaginationLink>
          </PaginationItem>,
        );
      }

      if (page < totalPageCount - 2) {
        items.push(
          <PaginationItem key="ellipsis-end">
            <PaginationEllipsis />
          </PaginationItem>,
        );
      }

      items.push(
        <PaginationItem key={totalPageCount}>
          <PaginationLink
            href="#"
            isActive={page === totalPageCount}
            onClick={() => buildLink(totalPageCount)}
          >
            {totalPageCount}
          </PaginationLink>
        </PaginationItem>,
      );
    }

    return items;
  };

  return (
    <div className="flex flex-col md:flex-row items-center gap-3 w-full">
      <div className="flex flex-col gap-4 flex-1">
        <SelectRowsPerPage
          options={pageSizeOptions}
          setPageSize={navToPageSize}
          pageSize={pageSize}
        />
      </div>
      <Pagination className={cn({ "md:justify-end": pageSizeOptions })}>
        <PaginationContent className="max-sm:gap-0">
          <PaginationItem>
            <PaginationPrevious
              href="#"
              aria-disabled={page === 1}
              tabIndex={page === 1 ? -1 : undefined}
              onClick={() => buildLink(Math.max(page - 1, 1))}
              className={
                page === 1 ? "pointer-events-none opacity-50" : undefined
              }
            />
          </PaginationItem>
          {renderPageNumbers()}
          <PaginationItem>
            <PaginationNext
              href="#"
              aria-disabled={page === totalPageCount}
              tabIndex={page === totalPageCount ? -1 : undefined}
              onClick={() => buildLink(Math.min(page + 1, totalPageCount))}
              className={
                page === totalPageCount
                  ? "pointer-events-none opacity-50"
                  : undefined
              }
            />
          </PaginationItem>
        </PaginationContent>
      </Pagination>
    </div>
  );
};

const SelectRowsPerPage = ({
  options,
  setPageSize,
  pageSize,
}: {
  options: number[];
  setPageSize: (newSize: number) => void;
  pageSize: number;
}) => {
  return (
    <div className="flex items-center gap-4">
      <span className="whitespace-nowrap text-sm">Zones per page</span>
      <Select
        value={String(pageSize)}
        onValueChange={(value) => setPageSize(Number(value))}
      >
        <SelectTrigger>
          <SelectValue placeholder="Select page size">
            {String(pageSize)}
          </SelectValue>
        </SelectTrigger>
        <SelectContent>
          {options.map((option) => (
            <SelectItem key={option} value={String(option)}>
              {option}
            </SelectItem>
          ))}
        </SelectContent>
      </Select>
    </div>
  );
};
